# Equipment

| Equipment | Description | Serial Number | Date of last calibration |
|-----------|-------------|---------------|--------------------------|
| Multimeter | Mastech M92 | 20011206033 | - |
| Power Supply | Siglent SPD3303C | SPD3EEEC6R0513 | TBD |

# Test Incident Reports

| Test | Incident | Proposed Solution | Status |
|------|----------|-------------------|--------|
| TIR1 | Mounting holes are too small | Increase hole size to ⌀3.2mm | Open |
| TIR2 | The two slots are too wide | * decrease slot width by 0.1mm<br/>* lengthen studs to 1.6mm | Open |
| TIR3 | Silkscreen is covered by the back panel | * Decrease font size of the silkscreen text "Power Input" so that it becomes readable <br/>* The current board design can be used, but it will look nicer when mounted with the silkscreen pointing inwards (not visible from the outside of the housing) | Open |
| TIR4 | Polarity of the power input | * Print the polarity of the wire-to-board connector on this PCB<br/>* On current protos, polarity can be marked with a permanent marker. | Open |
| TIR5 | Annular ring of the slots quite narrow | * Increase annular ring width to the same as that of the slot for the USB-C connector | Open |
| TIR6 | Power entry module is green, while back panel is silver colored | Remove soldermask on the outward pointing side and put text in green soldermask. | Open |

# Test Results
| Action | Expected Result | Observed result | Status |
|--------|-----------------|-----------------|--------|
| Mechanical fit unit in back panel | fits | mounting holes are too small | TIR1 |
| Mechanical fit of the two PCBs | fits | * The two slots are too wide<br/>* The two studs are 1mm long, which is too short. | TIR2 | Open |
| Silkscreen is legible | legible | silkscreen is covered by the back panel | TIR3 |
| Polarity of the power input | correct | it would be helpful to print the polarity of the wire-to-board connector on this PCB. | TIR4 |
| Solder connection between PCBs | sturdy connection | Annular ring of the slots quite narrow | TIR5 |
| USB-C powering | 5V on wire-to-board connector | 5V on wire-to-board connector | ✅ |
| Color matching of power entry to back panel| color matches | power entry module is green, while back panel is silver colored. | TIR6 |

# Test Result Details
