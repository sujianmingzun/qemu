/*
 * virtio-iommu device
 *
 * Copyright (c) 2017 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef QEMU_VIRTIO_IOMMU_H
#define QEMU_VIRTIO_IOMMU_H

#include "standard-headers/linux/virtio_iommu.h"
#include "hw/virtio/virtio.h"
#include "hw/pci/pci.h"

#define TYPE_VIRTIO_IOMMU "virtio-iommu-device"
#define VIRTIO_IOMMU(obj) \
        OBJECT_CHECK(VirtIOIOMMU, (obj), TYPE_VIRTIO_IOMMU)

#define IOMMU_PCI_BUS_MAX      256
#define IOMMU_PCI_DEVFN_MAX    256

typedef struct IOMMUDevice {
    void         *viommu;
    PCIBus       *bus;
    int           devfn;
    MemoryRegion  iommu_mr;
    AddressSpace  as;
} IOMMUDevice;

typedef struct IOMMUPciBus {
    PCIBus       *bus;
    IOMMUDevice  *pbdev[0]; /* Parent array is sparse, so dynamically alloc */
} IOMMUPciBus;

typedef struct VirtIOIOMMU {
    VirtIODevice parent_obj;
    VirtQueue *vq;
    struct virtio_iommu_config config;
    MemoryRegionIOMMUOps iommu_ops;
    uint32_t host_features;
    GHashTable *as_by_busptr;
    IOMMUPciBus *as_by_bus_num[IOMMU_PCI_BUS_MAX];
    GTree *address_spaces;
    QemuMutex mutex;
    GTree *devices;
} VirtIOIOMMU;

#endif
