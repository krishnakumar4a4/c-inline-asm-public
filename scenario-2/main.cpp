void test() {
    asm volatile(R"(
.L%=:
.pushsection .debug$S,"dr"
.long 4                                      # CodeView signature
.long 0xF1                                   # DEBUG_S_SYMBOLS subsection
.long .Lsymbols_end1-.Lsymbols_start1           # Subsection size
.Lsymbols_start1:
.short  3f-2f # Record length
2:
.short  4121 # Record kind: S_ANNOTATION
.secrel32 .L%=
.secidx .L%=
.short  3
.asciz  "b1"
.asciz  "b2"
.asciz  "b3"
.p2align 2, 0x0
3:
.Lsymbols_end1:
.popsection
)":);

    asm volatile(R"(
.L%=:
.pushsection .debug$S,"dr"
.long 4                                      # CodeView signature
.long 0xF1                                   # DEBUG_S_SYMBOLS subsection
.long .Lsymbols_end2-.Lsymbols_start2           # Subsection size
.Lsymbols_start2:
.short  3f-2f # Record length
2:
.short  4121 # Record kind: S_ANNOTATION
.secrel32 .L%=
.secidx .L%=
.short  3
.asciz  "c1"
.asciz  "c2"
.asciz  "c3"
.p2align 2, 0x0
3:
.Lsymbols_end2:
.popsection
)":);
}