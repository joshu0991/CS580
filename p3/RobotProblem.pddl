(define (problem Carry-1)
(:domain carrying-objects)
(:Objects R1 R2 R3 R4 R5 R6 R7 R8 R9 O1 O2 O3 B)
(:init
    (at R1)
    (have R6 O1) (have R4 O2) (have R7 O3)
    (connect R1 R4) (connect R4 R5) (connect R5 R6) (connect R6 R3) (connect R6 R9) (connect R2 R3) (connect R5 R8) (connect R8 R7)
    (connect R4 R1) (connect R5 R4) (connect R6 R5) (connect R3 R6) (connect R9 R6) (connect R3 R2) (connect R8 R5) (connect R7 R8)
    (is-room R1) (is-room R2) (is-room R3) (is-room R4) (is-room R5) (is-room R6) (is-room R7) (is-room R8) (is-room R9)
    (is-object O1) (is-object O2) (is-object O3)
    (empty B) (empty R1) (empty R2) (empty R3) (empty R5) (empty R8) (empty R9)
)
(:goal (and (have R7 O1) (have R2 O2) (have R9 O3))))