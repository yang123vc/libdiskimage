
/**
 * libdiskimage
 * VDI Block Storage
 * (C) 2012 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Accesses a VDI block storage
 */

#include "DIVDIBlockStorage.h"

#define HEADER_SIZE 0x200

DIVDIBlockStorage::DIVDIBlockStorage()
{
    close();
}

bool DIVDIBlockStorage::open(DIBackingStore *backingStore)
{
    close();
    
    DIChar header[HEADER_SIZE];
    
    if (!backingStore->read(0, header, HEADER_SIZE))
        return false;
    
    // Check id
    if (getDIIntLE(&header[0x40]) != 0x7f10dabe)
        return false;
    
    // Check VDI version
    if (getDIIntLE(&header[0x44]) > 0x00010001)
        return false;
    
    // Check VDI type
    type = getDIIntLE(&header[0x4c]);
    if ((type != DI_VDI_DYNAMIC) &&
        (type != DI_VDI_STATIC))
        return false;
    
    // Get flags
    DIInt flags = getDIIntLE(&header[0x50]);
    
    // Get image location
    offsetBlocks = getDIIntLE(&header[0x154]);
    offsetData = getDIIntLE(&header[0x158]);
    
    // Get CHS data
    cylinders = getDIIntLE(&header[0x15c]);
    heads = getDIIntLE(&header[0x160]);
    sectors = getDIIntLE(&header[0x164]);
    
    // Check sector size
    if (getDIIntLE(&header[0x168]) != 0x200)
        return false;
    
    diskSize = getDILongLE(&header[0x170]);
    blockSize = getDIIntLE(&header[0x178]);
    blockExtraData = getDIIntLE(&header[0x17c]);
    
    blocksInHDD = getDIIntLE(&header[0x180]);
    blocksAllocated = getDIIntLE(&header[0x184]);
    
    // Check last snapshot UUID to be zero
    if (getDILongLE(&header[0x198]) || getDILongLE(&header[0x1a0]))
        return false;
    
    // Check link UUID to be zero
    if (getDILongLE(&header[0x1a8]) || getDILongLE(&header[0x1b0]))
        return false;
    
    // Check parent UUID to be zero
    if (getDILongLE(&header[0x1b8]) || getDILongLE(&header[0x1c0]))
        return false;
    
    this->backingStore = backingStore;
    
    return true;
}

void DIVDIBlockStorage::close()
{
}

bool DIVDIBlockStorage::isWriteEnabled()
{
    return false;
}

DIInt DIVDIBlockStorage::getBlockNum()
{
    return 0;
}

string DIVDIBlockStorage::getFormatLabel()
{
    string formatLabel = "VDI Disk Image";
    
    if (!isWriteEnabled())
        formatLabel += " (read-only)";
    
    return formatLabel;
}

bool DIVDIBlockStorage::readBlocks(DIInt index, DIChar *buf, DIInt num)
{
    // To-Do
    
    return false;
}

bool DIVDIBlockStorage::writeBlocks(DIInt index, const DIChar *buf, DIInt num)
{
    // To-Do
    
    return false;
}
