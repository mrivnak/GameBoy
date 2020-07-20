/*
--------------------------------------------------------------------------------

        File: dmg_rom.bin
        File created with Hopper 4.4.7
        Analysis version 55
        Loader: RAW
        CPU: Sharp/LR35902
        64 bits (Little Endian)

--------------------------------------------------------------------------------
*/



        ; Segment Boot ROM
        ; Range: [0x0; 0x100[ (256 bytes)
        ; File offset : [0; 256[ (256 bytes)
        ; Permissions: readable / writable / executable



        ; Section
        ; Range: [0x0; 0x100[ (256 bytes)
        ; File offset : [0; 256[ (256 bytes)



        ; ================ B E G I N N I N G   O F   P R O C E D U R E ================


             Boot:
00000000         ld         sp, $fffe                                           ; Set up the stack
00000003         xor        a
00000004         ld         hl, $9fff                                           ; Zero VRAM from $9fff - $8000

             loc_7:
00000007         ld         [hl-], a                                            ; CODE XREF=Boot+10
00000008         bit        7, h
0000000a         jr         nz, @-$05                                           ; 0x7

0000000c         ld         hl, rNR52
0000000f         ld         c, $11
00000011         ld         a, $80
00000013         ld         [hl-], a                                            ; rNR52 = $80, Enable sound
00000014         ld         [c], a                                              ; rNR11 = $80
00000015         inc        c
00000016         ld         a, $f3
00000018         ld         [c], a                                              ; rNR12 = $f3
00000019         ld         [hl-], a                                            ; rNR51 = $f3
0000001a         ld         a, $77
0000001c         ld         [hl], a                                             ; rNR50 = $77
0000001d         ld         a, $fc
0000001f         ldh        [rBGP], a                                           ; Set color 0 to white and colors 1 - 3 to black
00000021         ld         de, $0104                                           ; de = cartridge header logo
00000024         ld         hl, $8010                                           ; hl = VRAM

             loc_27:
00000027         ld         a, [de]                                             ; for loop over cartridge logo data, CODE XREF=Boot+50
00000028         call       $0095                                               ; Copy cartridge logo data to VRAM at $8010, CopyData
0000002b         call       $0096
0000002e         inc        de
0000002f         ld         a, e
00000030         cp         $34
00000032         jr         nz, @-$0d                                           ; 0x27

00000034         ld         de, $00d8                                           ; de = boot rom data after logo
00000037         ld         b, $08                                              ; b = length of data

             loc_39:
00000039         ld         a, [de]                                             ; CODE XREF=Boot+62
0000003a         inc        de
0000003b         ld         [hl+], a                                            ; Write additional data into VRAM. Copyright logo?
0000003c         inc        hl
0000003d         dec        b
0000003e         jr         nz, @-$07                                           ; 0x39

00000040         ld         a, $19
00000042         ld         [$9910], a
00000045         ld         hl, $992f

             loc_48:
00000048         ld         c, $0c                                              ; CODE XREF=Boot+83

             loc_4a:
0000004a         dec        a                                                   ; CODE XREF=Boot+79
0000004b         jr         z, @+$08                                            ; 0x55

0000004d         ld         [hl-], a
0000004e         dec        c
0000004f         jr         nz, @-$07                                           ; 0x4a

00000051         ld         l, $0f
00000053         jr         @-$0d                                               ; 0x48

             loc_55:
00000055         ld         h, a                                                ; h = 0, CODE XREF=Boot+75
00000056         ld         a, $64
00000058         ld         d, a
00000059         ldh        [rSCY], a                                           ; SCY = $64
0000005b         ld         a, $91
                             ; rLCDC = b10010001
                             ; LCD enabled
                             ; BG data select 1 8000-8FFF
                             ; BG display enabled
0000005d         ldh        [rLCDC], a
0000005f         inc        b                                                   ; b = 1

             loc_60:
00000060         ld         e, $02                                              ; CODE XREF=Boot+140, Boot+147

             loc_62:
00000062         ld         c, $0c                                              ; CODE XREF=Boot+110

             loc_64:
00000064         ldh        a, [rLY]                                            ; Wait for frame, CODE XREF=Boot+104, Boot+107
00000066         cp         $90
00000068         jr         nz, @-$06                                           ; 0x64

0000006a         dec        c
0000006b         jr         nz, @-$09                                           ; 0x64

0000006d         dec        e
0000006e         jr         nz, @-$0e                                           ; 0x62

00000070         ld         c, $13
00000072         inc        h
00000073         ld         a, h
00000074         ld         e, $83
00000076         cp         $62
00000078         jr         z, @+$06                                            ; 0x80

0000007a         ld         e, $c1
0000007c         cp         $64
0000007e         jr         nz, @+$06                                           ; 0x86

             loc_80:
00000080         ld         a, e                                                ; CODE XREF=Boot+120
00000081         ld         [c], a
00000082         inc        c
00000083         ld         a, $87
00000085         ld         [c], a

             loc_86:
00000086         ldh        a, [rSCY]                                           ; CODE XREF=Boot+126
00000088         sub        b
00000089         ldh        [rSCY], a                                           ; Scroll the Nintendo Logo on screen
0000008b         dec        d
0000008c         jr         nz, @-$2e                                           ; 0x60

0000008e         dec        b
0000008f         jr         nz, @+$4f                                           ; Jump to Nintendo Logo check, 0xe0

00000091         ld         d, $20
00000093         jr         @-$35                                               ; 0x60


        ; ================ B E G I N N I N G   O F   P R O C E D U R E ================


             CopyData:
00000095         ld         c, a                                                ; CODE XREF=Boot+40
00000096         ld         b, $04                                              ; CODE XREF=Boot+43

             loc_98:
00000098         push       bc                                                  ; CODE XREF=CopyData+12
00000099         rl         c
0000009b         rla
0000009c         pop        bc
0000009d         rl         c
0000009f         rla
000000a0         dec        b
000000a1         jr         nz, @-$0b                                           ; 0x98

000000a3         ld         [hl+], a
000000a4         inc        hl
000000a5         ld         [hl+], a
000000a6         inc        hl
000000a7         ret
                        ; endp
             NintendoLogo:
000000a8         db         0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 
000000b8         db         0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
000000c8         db         0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
             Copyright:
000000d8         db         0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c

             loc_e0:
000000e0         ld         hl, $0104                                           ; hl = rom cartridge header logo, CODE XREF=Boot+143
000000e3         ld         de, $00a8                                           ; de = boot rom logo

             loc_e6:
000000e6         ld         a, [de]                                             ; for loop over the cartridge header logo, CODE XREF=CopyData+90
000000e7         inc        de
000000e8         cp         [hl]

             loc_e9:
000000e9         jr         nz, @-$02                                           ; If a byte in the cartridge logo doesn’t equal the boot room then loop forever, 0xe9, CODE XREF=loc_e9

000000eb         inc        hl
000000ec         ld         a, l
000000ed         cp         $34
000000ef         jr         nz, @-$0b                                           ; 0xe6

000000f1         ld         b, $19
000000f3         ld         a, b

             loc_f4:
000000f4         add        [hl]                                                ; for loop through the rest of the header to calculate checksum, CODE XREF=CopyData+98
000000f5         inc        hl
000000f6         dec        b
000000f7         jr         nz, @-$05                                           ; 0xf4

000000f9         add        [hl]                                                ; Validate against the cartridge header checksum field

             loc_fa:
000000fa         jr         nz, @-$02                                           ; If header checksum is invalid then loop forever, 0xfa, CODE XREF=loc_fa

000000fc         ld         a, $01
000000fe         ldh        [$ff00+$50], a                                      ; Write 1 to $ff50 to disable boot ROM
                        ; endp


        ; Segment Cartridge ROM
        ; Range: [0x100; 0x150[ (80 bytes)
        ; No data on disk
        ; Permissions:  - 

00000100         nop                                                            ; Execution continues here in cartridge from boot ROM
00000101         jp         $0150
                             ; Logo
00000104         db         0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 
00000114         db         0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 
00000124         db         0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
                             ; Title
00000134         db  0x54 ; 'T'
00000135         db  0x45 ; 'I'
00000136         db  0x54 ; 'T'
00000137         db  0x52 ; 'L'
00000138         db  0x49 ; 'E'
00000139         db  0x00 ; '.'
0000013a         db  0x00 ; '.'
0000013b         db  0x00 ; '.'
0000013c         db  0x00 ; '.'
0000013d         db  0x00 ; '.'
0000013e         db  0x00 ; '.'
0000013f         db  0x00 ; '.'
00000140         db  0x00 ; '.'
00000141         db  0x00 ; '.'
00000142         db  0x00 ; '.'
00000143         db  0x00 ; '.'
                             ; New Licensee Code
00000144         dw         0x0000
                             ; SGB Flag
00000146         db  0x00 ; '.'
                             ; Cartridge Type
00000147         db  0x00 ; '.'
                             ; ROM Size
00000148         db  0x00 ; '.'
                             ; RAM Size
00000149         db  0x00 ; '.'
                             ; Destination Code
0000014a         db  0x00 ; '.'
                             ; Old Licensee Code
0000014b         db  0x01 ; '.'
                             ; Mask ROM Version number
0000014c         db  0x01 ; '.'
                             ; Header Checksum
0000014d         db  0x0a ; '.'
                             ; Global Checksum
0000014e         dw         0xbf16
