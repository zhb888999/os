#include <arch/x86_64.h>

inline void set_segment_code(SegmentDescriptor64 *des, uint16_t attr) {
    des->ignore0 = 0;
    des->ignore1 = 0;
    des->attributes = set_code_segment_attr(attr);
    des->ignore2 = 0;
}

inline void set_segment_data(SegmentDescriptor64 *des, uint16_t attr) {
    des->ignore0 = 0;
    des->ignore1 = 0;
    des->attributes = set_data_segment_attr(attr);
    des->ignore2 = 0;
}

inline void set_segment_tss(SegmentTSSDescriptor128 *des, uint64_t baseaddr, uint32_t length, uint16_t attr) {
    des->length_1 = length & 0xffff;
    des->baseaddr_1 = baseaddr & 0xffff;
    des->baseaddr_2 = (baseaddr & 0xff0000) >> 16;
    des->attributes_length_2 = ((length & 0xf0000) >> 8) | set_tss_segment_attr(attr);
    des->baseaddr_3 = (baseaddr & 0xff000000) >> 24;
    des->baseaddr_4 = baseaddr >> 32;
    des->zero = 0;
}

inline void set_segment_cgd(SegmentCGDDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t attr) {
    des->offset_1 = offset << 48 >> 48;
    des->selector = selector;
    des->zero_must = 0;
    des->attributes = set_cgd_segment_attr(attr); 
    des->offset_2 = offset << 32 >> 48;
    des->offset_3 = offset >> 32;
    des->zero = 0;
}

inline void set_idt(InterruptDescriptor128 *des, uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attributes) {
    des->offset_1 = offset << 48 >> 48;
    des->selector = selector;
    des->zero = 0;
    des->type_attributes = set_cgd_segment_attr(type_attributes); 
    des->offset_2 = offset << 32 >> 48;
    des->offset_3 = offset >> 32;
    des->zero = 0;
}