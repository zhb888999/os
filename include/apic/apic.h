#ifndef APIC_APIC_H
#define APIC_APIC_H

#include <int.h>

typedef struct {
	unsigned int 	vector	:8,	//0~7
		deliver_mode	:3,	//8~10
		dest_mode	:1,	//11
		deliver_status	:1,	//12
			polarity:1,	//13
			irr	:1,	//14
			trigger	:1,	//15
			mask	:1,	//16
			reserved:15;	//17~31

	union{
		struct {
			unsigned int reserved1	:24,	//32~55
				phy_dest	:4,	//56~59
				     reserved2	:4;	//60~63
			}physical;

		struct {
			unsigned int reserved1	:24,	//32~55
				logical_dest	:8;	//56~63
			}logical;
		}destination;
}__attribute__((packed)) IRQEntry;

void init_localapic(void);
void init_ioapic(void);
void start_up(void);
void enable_ioapic(void);
void enable_imcr(void);

uint32_t read_ioapic_rte32(uint8_t index);
uint64_t read_ioapic_rte64(uint8_t index);
void write_ioapic_rte32(uint8_t index, uint32_t value);
void write_ioapic_rte64(uint8_t index, uint64_t value);

void irq_enable(uint64_t irq);
void irq_disable(uint64_t irq);
uint64_t irq_install(uint64_t irq, void *arg);
void irq_uninstall(uint64_t irq);
void irq_level_ack(uint64_t irq);
void irq_edge_ack(uint64_t irq);

//delivery mode
#define	APIC_ICR_IOAPIC_Fixed 		 0	//LAPIC	IOAPIC 	ICR
#define	IOAPIC_ICR_Lowest_Priority 	 1	//	IOAPIC 	ICR
#define	APIC_ICR_IOAPIC_SMI		 2	//LAPIC	IOAPIC 	ICR

#define	APIC_ICR_IOAPIC_NMI		 4	//LAPIC	IOAPIC 	ICR
#define	APIC_ICR_IOAPIC_INIT		 5	//LAPIC	IOAPIC 	ICR
#define	ICR_Start_up			 6	//		ICR
#define	IOAPIC_ExtINT			 7	//	IOAPIC


/*

*/
//timer mode
#define APIC_LVT_Timer_One_Shot		0
#define APIC_LVT_Timer_Periodic		1
#define APIC_LVT_Timer_TSC_Deadline	2

//mask
#define APIC_ICR_IOAPIC_Masked		1
#define APIC_ICR_IOAPIC_UN_Masked	0

//trigger mode
#define APIC_ICR_IOAPIC_Edge		0
#define APIC_ICR_IOAPIC_Level		1

//delivery status
#define APIC_ICR_IOAPIC_Idle		0
#define APIC_ICR_IOAPIC_Send_Pending	1

//destination shorthand
#define ICR_No_Shorthand		0
#define ICR_Self			1
#define ICR_ALL_INCLUDE_Self		2
#define ICR_ALL_EXCLUDE_Self		3

//destination mode
#define ICR_IOAPIC_DELV_PHYSICAL	0
#define ICR_IOAPIC_DELV_LOGIC		1

//level
#define ICR_LEVEL_DE_ASSERT		0
#define ICR_LEVLE_ASSERT		1

//remote irr
#define APIC_IOAPIC_IRR_RESET		0
#define APIC_IOAPIC_IRR_ACCEPT		1

//pin polarity
#define APIC_IOAPIC_POLARITY_HIGH	0
#define APIC_IOAPIC_POLARITY_LOW	1

#define irq2rte(x) ((x) * 2 + 0x10)
#define irq2vector(x) ((x) + 0x20)

#define local_eoi() \
do{ \
	__asm__ __volatile__(	\
        "movq	$0x00,	%%rdx	\n\t" \
		"movq	$0x00,	%%rax	\n\t" \
		"movq 	$0x80b,	%%rcx	\n\t" \
		"wrmsr	\n\t" \
		:::"memory" \
    ); \
}while(0)

#endif