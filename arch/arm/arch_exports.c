/*
 *  ARM interface functions.
 *
 *  Copyright (c) Antmicro
 *  Copyright (c) Realtime Embedded
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include "cpu.h"
#include "../../unwind.h"

uint32_t tlib_get_cpu_id()
{
    return cpu->cp15.c0_cpuid;
}

EXC_INT_0(uint32_t, tlib_get_cpu_id)

uint32_t tlib_get_it_state()
{
    return cpu->condexec_bits;
}

EXC_INT_0(uint32_t, tlib_get_it_state)

uint32_t tlib_evaluate_condition_code(uint32_t condition)
{
    uint8_t ZF = (env->ZF == 0);
    uint8_t NF = (env->NF & 0x80000000) > 0;
    uint8_t CF = (env->CF == 1);
    uint8_t VF = (env->VF & 0x80000000) > 0;
    switch (condition) {
    case 0b0000:        //EQ
        return ZF == 1;
    case 0b0001:        //NE
        return ZF == 0;
    case 0b0010:        //CS
        return CF == 1;
    case 0b0011:        //CC
        return CF == 0;
    case 0b0100:        //MI
        return NF == 1;
    case 0b0101:        //PL
        return NF == 0;
    case 0b0110:        //VS
        return VF == 1;
    case 0b0111:        //VC
        return VF == 0;
    case 0b1000:        //HI
        return CF == 1 && ZF == 0;
    case 0b1001:        //LS
        return CF == 0 || ZF == 1;
    case 0b1010:        //GE
        return NF == VF;
    case 0b1011:        //LT
        return NF != VF;
    case 0b1100:        //GT
        return ZF == 0 && NF == VF;
    case 0b1101:        //LE
        return ZF == 1 || NF != VF;
    case 0b1110:        //AL
        return 1;
    case 0b1111:        //NV
        return 0;
    default:
        tlib_printf(LOG_LEVEL_ERROR, "trying to evaluate incorrect condition code (0x%x)", condition);
        return 0;
    }
}

EXC_INT_1(uint32_t, tlib_evaluate_condition_code, uint32_t, condition)

void tlib_set_cpu_id(uint32_t value)
{
    cpu->cp15.c0_cpuid = value;
}

EXC_VOID_1(tlib_set_cpu_id, uint32_t, value)

void tlib_toggle_fpu(int32_t enabled)
{
    if (enabled) {
        cpu->vfp.xregs[ARM_VFP_FPEXC] |= ARM_VFP_FPEXC_FPUEN_MASK;
    } else {
        cpu->vfp.xregs[ARM_VFP_FPEXC] &= ~ARM_VFP_FPEXC_FPUEN_MASK;
    }
}

EXC_VOID_1(tlib_toggle_fpu, int32_t, enabled)

void tlib_set_sev_on_pending(int32_t value)
{
    cpu->sev_on_pending = !!value;
}

EXC_VOID_1(tlib_set_sev_on_pending, int32_t, value)

void tlib_set_event_flag(int value)
{
    cpu->sev_pending = !!value;
}

EXC_VOID_1(tlib_set_event_flag, int, value)

void tlib_set_thumb(int value)
{
    cpu->thumb = value != 0;
}

EXC_VOID_1(tlib_set_thumb, int, value)

#ifdef TARGET_PROTO_ARM_M

void tlib_set_interrupt_vector_base(uint32_t address)
{
    cpu->v7m.vecbase = address;
}

EXC_VOID_1(tlib_set_interrupt_vector_base, uint32_t, address)

uint32_t tlib_get_interrupt_vector_base()
{
    return cpu->v7m.vecbase;
}

EXC_INT_0(uint32_t, tlib_get_interrupt_vector_base)

uint32_t tlib_get_xpsr()
{
    return xpsr_read(cpu);
}

EXC_INT_0(uint32_t, tlib_get_xpsr)

uint32_t tlib_get_fault_status()
{
    return cpu->v7m.fault_status;
}

EXC_INT_0(uint32_t, tlib_get_fault_status)

void tlib_set_fault_status(uint32_t value)
{
    cpu->v7m.fault_status = value;
}

EXC_VOID_1(tlib_set_fault_status, uint32_t, value)

uint32_t tlib_get_memory_fault_address()
{
    return cpu->cp15.c6_data;
}

EXC_INT_0(uint32_t, tlib_get_memory_fault_address)

uint32_t tlib_is_mpu_enabled()
{
    return cpu->cp15.c1_sys & 0x1;
}

EXC_INT_0(uint32_t, tlib_is_mpu_enabled)

void tlib_enable_mpu(int32_t enabled)
{
    cpu->cp15.c1_sys |= enabled ? 1 : 0;
    tlb_flush(cpu, 1);
}

EXC_VOID_1(tlib_enable_mpu, int32_t, enabled)

// This function mimics mpu configuration through the "Region Base Address" register
void tlib_set_mpu_region_base_address(uint32_t value)
{
    if (value & 0x10) {
        /* If VALID (0x10) bit is set, we change the region number to zero-extended value of youngest 4 bits */
        cpu->cp15.c6_region_number = value & 0xF;
    }
    cpu->cp15.c6_base_address[cpu->cp15.c6_region_number] = value & 0xFFFFFFE0;
    tlib_printf(LOG_LEVEL_DEBUG, "MPU: Set base address 0x%x, for region %lld", value & 0xFFFFFFE0, cpu->cp15.c6_region_number);
    tlb_flush(cpu, 1);
}

EXC_VOID_1(tlib_set_mpu_region_base_address, uint32_t, value)

// This function mimics mpu configuration through the "Region Attribute and Size" register
void tlib_set_mpu_region_size_and_enable(uint32_t value)
{
    uint32_t index = cpu->cp15.c6_region_number;
    cpu->cp15.c6_size_and_enable[index] = value & MPU_SIZE_AND_ENABLE_FIELD_MASK;
    cpu->cp15.c6_subregion_disable[index] = (value & MPU_SUBREGION_DISABLE_FIELD_MASK) >> 8;
    cpu->cp15.c6_access_control[index] = value >> 16;
    tlib_printf(LOG_LEVEL_DEBUG, "MPU: Set access control 0x%x, permissions 0x%x, size 0x%x, enable 0x%x, for region %lld", value >> 16, ((value >> 16) & MPU_PERMISSION_FIELD_MASK) >> 8 , (value & MPU_SIZE_FIELD_MASK) >> 1, value & MPU_REGION_ENABLED_BIT, index);
    tlb_flush(cpu, 1);
}

EXC_VOID_1(tlib_set_mpu_region_size_and_enable, uint32_t, value)

void tlib_set_mpu_region_number(uint32_t value)
{
    if (value >= MAX_MPU_REGIONS) {
        tlib_printf(LOG_LEVEL_ERROR, "MPU: Trying to use non-existent MPU region. Number of regions: %d, faulting region number: %d", MAX_MPU_REGIONS, value);
        return;
    }
    cpu->cp15.c6_region_number = value;
    tlb_flush(cpu, 1);
}

EXC_VOID_1(tlib_set_mpu_region_number, uint32_t, value)

// This function mimics mpu configuration through the "Region Base Address" register
uint32_t tlib_get_mpu_region_base_address()
{
    return cpu->cp15.c6_base_address[cpu->cp15.c6_region_number] | cpu->cp15.c6_region_number;
}

EXC_INT_0(uint32_t, tlib_get_mpu_region_base_address)

// This function mimics mpu configuration through the "Region Attribute and Size" register
uint32_t tlib_get_mpu_region_size_and_enable()
{
    uint32_t index = cpu->cp15.c6_region_number;
    return (cpu->cp15.c6_access_control[index] << 16) | (cpu->cp15.c6_subregion_disable[index] << 8) | cpu->cp15.c6_size_and_enable[index];
}

EXC_INT_0(uint32_t, tlib_get_mpu_region_size_and_enable)

uint32_t tlib_get_mpu_region_number()
{
    return cpu->cp15.c6_region_number;
}

EXC_INT_0(uint32_t, tlib_get_mpu_region_number)

#endif
