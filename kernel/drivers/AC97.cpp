#include <libsystem/Logger.h>

#include "kernel/drivers/AC97.h"

/*

Tasks

Done
- created driver class
- initialised constants
- allocated buffer memory
- initialised driver
- set max volume

Left
- set sample rate in initialise
- add logging to functions
- try to initialise driver in qemu
- implement write function that starts playing
- handle interrupt and filebuffer memory 
- stop playing once buffer is complete or call received
- implement record using read
- implement volume control

*/

AC97::AC97(DeviceAddress address) : PCIDevice(address, DeviceClass::SOUND)
{
    // set values
    // knobs = _knobs;
    playback_format = 0;
    playback_speed = 0;

    // get native audio bus master bar and native audio mixer bar
    nabmbar = pci_address().read16(AC97_NABMBAR) & ((uint32_t)-1) << 1;
    nambar = pci_address().read32(PCI_BAR0) & ((uint32_t)-1) << 1;

    //  enable all device interrupts
    out8(nabmbar + AC97_PO_CR, AC97_X_CR_FEIE | AC97_X_CR_IOCE);

    // default the pcm output to full volume
    out16(nambar + AC97_PCM_OUT_VOLUME, 0x0000);
    initialise_buffers();

    // tell the ac97 where buffer descriptor list is
    out32(nabmbar + AC97_PO_BDBAR, buffer_descriptors_range->physical_base());
    // set last valid index
    lvi = 2;
    out8(nabmbar + AC97_PO_LVI, lvi);

    // detect wheter device supports MSB
    out32(nambar + AC97_MASTER_VOLUME, 0x2020);
    uint16_t t = in32(nambar + AC97_MASTER_VOLUME) & 0x1f;
    if (t == 0x1f)
    {
        // debug_print(WARNING, "This device only supports 5 bits of audio volume.");
        bits = 5;
        mask = 0x1f;
        out32(nambar + AC97_MASTER_VOLUME, 0x0f0f);
    }
    else
    {
        bits = 6;
        mask = 0x3f;
        out32(nambar + AC97_MASTER_VOLUME, 0x1f1f);
    }

    out8(nabmbar + AC97_PO_CR, in8(nabmbar + AC97_PO_CR) | AC97_X_CR_RPBM);

    // debug_print(NOTICE, "AC97 initialized successfully");

    //start device playing
}

void AC97::initialise_buffers()
{
    buffer_descriptors_range = make<MMIORange>(sizeof(AC97BufferDescriptor) * AC97_BDL_LEN);
    buffer_descriptors_list = reinterpret_cast<AC97BufferDescriptor *>(buffer_descriptors_range->base());

    for (size_t i = 0; i < AC97_BDL_LEN; i++)
    {
        buffers.push_back(make<MMIORange>(AC97_BDL_BUFFER_LEN));
        buffer_descriptors_list[i].pointer = buffers[i]->physical_base();
        AC97_CL_SET_LENGTH(buffer_descriptors_list[i].cl, AC97_BDL_BUFFER_LEN);
        /* Set all buffers to interrupt */
        buffer_descriptors_list[i].cl |= AC97_CL_IOC;
    }
}
