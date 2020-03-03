EESchema Schematic File Version 4
LIBS:ZXShield-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ZXShield"
Date "2020-02-01"
Rev ""
Comp "© 2020 Agustín Gimenez Bernad Licensed under CC-NC-SA"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x10 P5
U 1 1 56D72368
P 4900 1150
F 0 "P5" H 4900 1650 50  0000 C CNN
F 1 "PWM" V 5000 1150 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 4900 1150 50  0001 C CNN
F 3 "" H 4900 1150 50  0000 C CNN
	1    4900 1150
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P6
U 1 1 56D734D0
P 5750 1050
F 0 "P6" H 5750 1450 50  0000 C CNN
F 1 "PWM" V 5850 1050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5750 1050 50  0001 C CNN
F 3 "" H 5750 1050 50  0000 C CNN
	1    5750 1050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P7
U 1 1 56D73F2C
P 6650 1050
F 0 "P7" H 6650 1450 50  0000 C CNN
F 1 "Communication" V 6750 1050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 6650 1050 50  0001 C CNN
F 3 "" H 6650 1050 50  0000 C CNN
	1    6650 1050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P4
U 1 1 56D73A0E
P 4050 1050
F 0 "P4" H 4050 1450 50  0000 C CNN
F 1 "Analog" V 4150 1050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4050 1050 50  0001 C CNN
F 3 "" H 4050 1050 50  0000 C CNN
	1    4050 1050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P3
U 1 1 56D72F1C
P 3200 1050
F 0 "P3" H 3200 1450 50  0000 C CNN
F 1 "Analog" V 3300 1050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 3200 1050 50  0001 C CNN
F 3 "" H 3200 1050 50  0000 C CNN
	1    3200 1050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P2
U 1 1 56D71773
P 2300 1050
F 0 "P2" H 2300 1450 50  0000 C CNN
F 1 "Power" V 2400 1050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 2300 1050 50  0001 C CNN
F 3 "" H 2300 1050 50  0000 C CNN
	1    2300 1050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1100 750  750  750 
Wire Wire Line
	1100 850  750  850 
Wire Wire Line
	1100 950  750  950 
Wire Wire Line
	1100 1050 750  1050
Wire Wire Line
	1100 1150 750  1150
Wire Wire Line
	1100 1250 750  1250
Wire Wire Line
	1100 1350 750  1350
Wire Wire Line
	1100 1450 750  1450
Wire Wire Line
	1100 1550 750  1550
Wire Wire Line
	1100 1650 750  1650
Wire Wire Line
	1100 1750 750  1750
Wire Wire Line
	1100 1850 750  1850
Wire Wire Line
	1100 1950 750  1950
Wire Wire Line
	1100 2050 750  2050
Wire Wire Line
	1100 2150 750  2150
Wire Wire Line
	1100 2250 750  2250
Wire Wire Line
	1100 2350 750  2350
Wire Wire Line
	1100 2450 750  2450
Wire Wire Line
	1950 2450 1600 2450
Text Label 1050 750  2    50   ~ 0
PDIG_01
Wire Wire Line
	1950 2350 1600 2350
Wire Wire Line
	1950 2250 1600 2250
Wire Wire Line
	1950 2150 1600 2150
Wire Wire Line
	1950 2050 1600 2050
Wire Wire Line
	1950 1950 1600 1950
Wire Wire Line
	1950 1850 1600 1850
Wire Wire Line
	1950 1750 1600 1750
Wire Wire Line
	1950 1650 1600 1650
Wire Wire Line
	1950 1550 1600 1550
Wire Wire Line
	1950 1450 1600 1450
Wire Wire Line
	1950 1350 1600 1350
Wire Wire Line
	1950 1250 1600 1250
Wire Wire Line
	1950 1150 1600 1150
Wire Wire Line
	1950 1050 1600 1050
Wire Wire Line
	1950 950  1600 950 
Wire Wire Line
	1950 850  1600 850 
Wire Wire Line
	1950 750  1600 750 
$Comp
L Connector_Generic:Conn_02x18_Odd_Even P1
U 1 1 56D743B5
P 1300 1550
F 0 "P1" H 1350 1300 50  0000 C CNN
F 1 "Digital" V 1350 1550 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x18_P2.54mm_Vertical" H 1300 500 50  0001 C CNN
F 3 "" H 1300 500 50  0000 C CNN
	1    1300 1550
	1    0    0    -1  
$EndComp
Text Label 1050 850  2    50   ~ 0
PDIG_03
Text Label 1050 950  2    50   ~ 0
PDIG_05
Text Label 1050 1050 2    50   ~ 0
PDIG_07
Text Label 1050 1150 2    50   ~ 0
PDIG_09
Text Label 1050 1250 2    50   ~ 0
PDIG_11
Text Label 1050 1350 2    50   ~ 0
PDIG_13
Text Label 1050 1450 2    50   ~ 0
PDIG_15
Text Label 1050 1550 2    50   ~ 0
PDIG_17
Text Label 1050 1650 2    50   ~ 0
PDIG_19
Text Label 1050 1750 2    50   ~ 0
PDIG_21
Text Label 1050 1850 2    50   ~ 0
PDIG_23
Text Label 1050 1950 2    50   ~ 0
PDIG_25
Text Label 1050 2050 2    50   ~ 0
PDIG_27
Text Label 1050 2150 2    50   ~ 0
PDIG_29
Text Label 1050 2250 2    50   ~ 0
PDIG_31
Text Label 1050 2350 2    50   ~ 0
PDIG_33
Text Label 1050 2450 2    50   ~ 0
PDIG_35
Text Label 1650 750  0    50   ~ 0
PDIG_02
Text Label 1650 850  0    50   ~ 0
PDIG_04
Text Label 1650 950  0    50   ~ 0
PDIG_06
Text Label 1650 1050 0    50   ~ 0
PDIG_08
Text Label 1650 1150 0    50   ~ 0
PDIG_10
Text Label 1650 1250 0    50   ~ 0
PDIG_12
Text Label 1650 1350 0    50   ~ 0
PDIG_14
Text Label 1650 1450 0    50   ~ 0
PDIG_16
Text Label 1650 1550 0    50   ~ 0
PDIG_18
Text Label 1650 1650 0    50   ~ 0
PDIG_20
Text Label 1650 1750 0    50   ~ 0
PDIG_22
Text Label 1650 1850 0    50   ~ 0
PDIG_24
Text Label 1650 1950 0    50   ~ 0
PDIG_26
Text Label 1650 2050 0    50   ~ 0
PDIG_28
Text Label 1650 2150 0    50   ~ 0
PDIG_30
Text Label 1650 2250 0    50   ~ 0
PDIG_32
Text Label 1650 2350 0    50   ~ 0
PDIG_34
Text Label 1650 2450 0    50   ~ 0
PDIG_36
Wire Wire Line
	2850 1150 2500 1150
Wire Wire Line
	2850 1050 2500 1050
Wire Wire Line
	2850 950  2500 950 
Wire Wire Line
	2850 850  2500 850 
Wire Wire Line
	2850 750  2500 750 
Text Label 2550 750  0    50   ~ 0
PPOW_01
Text Label 2550 850  0    50   ~ 0
PPOW_02
Text Label 2550 950  0    50   ~ 0
PPOW_03
Text Label 2550 1050 0    50   ~ 0
PPOW_04
Text Label 2550 1150 0    50   ~ 0
PPOW_05
Wire Wire Line
	2850 1450 2500 1450
Wire Wire Line
	2850 1350 2500 1350
Wire Wire Line
	2850 1250 2500 1250
Text Label 2550 1250 0    50   ~ 0
PPOW_06
Text Label 2550 1350 0    50   ~ 0
PPOW_07
Text Label 2550 1450 0    50   ~ 0
PPOW_08
Wire Wire Line
	3750 1150 3400 1150
Wire Wire Line
	3750 1050 3400 1050
Wire Wire Line
	3750 950  3400 950 
Wire Wire Line
	3750 850  3400 850 
Wire Wire Line
	3750 750  3400 750 
Text Label 3450 750  0    50   ~ 0
PAN1_01
Text Label 3450 850  0    50   ~ 0
PAN1_02
Text Label 3450 950  0    50   ~ 0
PAN1_03
Text Label 3450 1050 0    50   ~ 0
PAN1_04
Text Label 3450 1150 0    50   ~ 0
PAN1_05
Wire Wire Line
	3750 1450 3400 1450
Wire Wire Line
	3750 1350 3400 1350
Wire Wire Line
	3750 1250 3400 1250
Text Label 3450 1250 0    50   ~ 0
PAN1_06
Text Label 3450 1350 0    50   ~ 0
PAN1_07
Text Label 3450 1450 0    50   ~ 0
PAN1_08
Wire Wire Line
	4600 1150 4250 1150
Wire Wire Line
	4600 1050 4250 1050
Wire Wire Line
	4600 950  4250 950 
Wire Wire Line
	4600 850  4250 850 
Wire Wire Line
	4600 750  4250 750 
Text Label 4300 750  0    50   ~ 0
PAN2_01
Text Label 4300 850  0    50   ~ 0
PAN2_02
Text Label 4300 950  0    50   ~ 0
PAN2_03
Text Label 4300 1050 0    50   ~ 0
PAN2_04
Text Label 4300 1150 0    50   ~ 0
PAN2_05
Wire Wire Line
	4600 1450 4250 1450
Wire Wire Line
	4600 1350 4250 1350
Wire Wire Line
	4600 1250 4250 1250
Text Label 4300 1250 0    50   ~ 0
PAN2_06
Text Label 4300 1350 0    50   ~ 0
PAN2_07
Text Label 4300 1450 0    50   ~ 0
PAN2_08
Wire Wire Line
	5450 1150 5100 1150
Wire Wire Line
	5450 1050 5100 1050
Wire Wire Line
	5450 950  5100 950 
Wire Wire Line
	5450 850  5100 850 
Wire Wire Line
	5450 750  5100 750 
Text Label 5150 750  0    50   ~ 0
PWM1_01
Text Label 5150 850  0    50   ~ 0
PWM1_02
Text Label 5150 950  0    50   ~ 0
PWM1_03
Text Label 5150 1050 0    50   ~ 0
PWM1_04
Text Label 5150 1150 0    50   ~ 0
PWM1_05
Wire Wire Line
	5450 1450 5100 1450
Wire Wire Line
	5450 1350 5100 1350
Wire Wire Line
	5450 1250 5100 1250
Text Label 5150 1250 0    50   ~ 0
PWM1_06
Text Label 5150 1350 0    50   ~ 0
PWM1_07
Text Label 5150 1450 0    50   ~ 0
PWM1_08
Wire Wire Line
	5450 1650 5100 1650
Wire Wire Line
	5450 1550 5100 1550
Text Label 5150 1550 0    50   ~ 0
PWM1_09
Text Label 5150 1650 0    50   ~ 0
PWM1_10
Wire Wire Line
	6300 1150 5950 1150
Wire Wire Line
	6300 1050 5950 1050
Wire Wire Line
	6300 950  5950 950 
Wire Wire Line
	6300 850  5950 850 
Wire Wire Line
	6300 750  5950 750 
Text Label 6000 750  0    50   ~ 0
PWM2_01
Text Label 6000 850  0    50   ~ 0
PWM2_02
Text Label 6000 950  0    50   ~ 0
PWM2_03
Text Label 6000 1050 0    50   ~ 0
PWM2_04
Text Label 6000 1150 0    50   ~ 0
PWM2_05
Wire Wire Line
	6300 1450 5950 1450
Wire Wire Line
	6300 1350 5950 1350
Wire Wire Line
	6300 1250 5950 1250
Text Label 6000 1250 0    50   ~ 0
PWM2_06
Text Label 6000 1350 0    50   ~ 0
PWM2_07
Text Label 6000 1450 0    50   ~ 0
PWM2_08
Wire Wire Line
	7200 1150 6850 1150
Wire Wire Line
	7200 1050 6850 1050
Wire Wire Line
	7200 950  6850 950 
Wire Wire Line
	7200 850  6850 850 
Wire Wire Line
	7200 750  6850 750 
Text Label 6900 750  0    50   ~ 0
COMM_01
Text Label 6900 850  0    50   ~ 0
COMM_02
Text Label 6900 950  0    50   ~ 0
COMM_03
Text Label 6900 1050 0    50   ~ 0
COMM_04
Text Label 6900 1150 0    50   ~ 0
COMM_05
Wire Wire Line
	7200 1450 6850 1450
Wire Wire Line
	7200 1350 6850 1350
Wire Wire Line
	7200 1250 6850 1250
Text Label 6900 1250 0    50   ~ 0
COMM_06
Text Label 6900 1350 0    50   ~ 0
COMM_07
Text Label 6900 1450 0    50   ~ 0
COMM_08
$Comp
L Connector_Generic:Conn_01x10 P12
U 1 1 5E3DFC5E
P 4850 2400
F 0 "P12" H 4850 2900 50  0000 C CNN
F 1 "PWM" V 4950 2400 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x10_P2.54mm_Vertical" H 4850 2400 50  0001 C CNN
F 3 "" H 4850 2400 50  0000 C CNN
	1    4850 2400
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P13
U 1 1 5E3DFC64
P 5700 2300
F 0 "P13" H 5700 2700 50  0000 C CNN
F 1 "PWM" V 5800 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 5700 2300 50  0001 C CNN
F 3 "" H 5700 2300 50  0000 C CNN
	1    5700 2300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P14
U 1 1 5E3DFC6A
P 6600 2300
F 0 "P14" H 6600 2700 50  0000 C CNN
F 1 "Communication" V 6700 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 6600 2300 50  0001 C CNN
F 3 "" H 6600 2300 50  0000 C CNN
	1    6600 2300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P11
U 1 1 5E3DFC70
P 4000 2300
F 0 "P11" H 4000 2700 50  0000 C CNN
F 1 "Analog" V 4100 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 4000 2300 50  0001 C CNN
F 3 "" H 4000 2300 50  0000 C CNN
	1    4000 2300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P10
U 1 1 5E3DFC76
P 3150 2300
F 0 "P10" H 3150 2700 50  0000 C CNN
F 1 "Analog" V 3250 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 3150 2300 50  0001 C CNN
F 3 "" H 3150 2300 50  0000 C CNN
	1    3150 2300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 P9
U 1 1 5E3DFC7C
P 2250 2300
F 0 "P9" H 2250 2700 50  0000 C CNN
F 1 "Power" V 2350 2300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 2250 2300 50  0001 C CNN
F 3 "" H 2250 2300 50  0000 C CNN
	1    2250 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1100 2700 750  2700
Wire Wire Line
	1100 2800 750  2800
Wire Wire Line
	1100 2900 750  2900
Wire Wire Line
	1100 3000 750  3000
Wire Wire Line
	1100 3100 750  3100
Wire Wire Line
	1100 3200 750  3200
Wire Wire Line
	1100 3300 750  3300
Wire Wire Line
	1100 3400 750  3400
Wire Wire Line
	1100 3500 750  3500
Wire Wire Line
	1100 3600 750  3600
Wire Wire Line
	1100 3700 750  3700
Wire Wire Line
	1100 3800 750  3800
Wire Wire Line
	1100 3900 750  3900
Wire Wire Line
	1100 4000 750  4000
Wire Wire Line
	1100 4100 750  4100
Wire Wire Line
	1100 4200 750  4200
Wire Wire Line
	1100 4300 750  4300
Wire Wire Line
	1100 4400 750  4400
Wire Wire Line
	1950 4400 1600 4400
Text Label 1050 2700 2    50   ~ 0
PDIG_01
Wire Wire Line
	1950 4300 1600 4300
Wire Wire Line
	1950 4200 1600 4200
Wire Wire Line
	1950 4100 1600 4100
Wire Wire Line
	1950 4000 1600 4000
Wire Wire Line
	1950 3900 1600 3900
Wire Wire Line
	1950 3800 1600 3800
Wire Wire Line
	1950 3700 1600 3700
Wire Wire Line
	1950 3600 1600 3600
Wire Wire Line
	1950 3500 1600 3500
Wire Wire Line
	1950 3400 1600 3400
Wire Wire Line
	1950 3300 1600 3300
Wire Wire Line
	1950 3200 1600 3200
Wire Wire Line
	1950 3100 1600 3100
Wire Wire Line
	1950 3000 1600 3000
Wire Wire Line
	1950 2900 1600 2900
Wire Wire Line
	1950 2800 1600 2800
Wire Wire Line
	1950 2700 1600 2700
$Comp
L Connector_Generic:Conn_02x18_Odd_Even P8
U 1 1 5E3DFCA7
P 1300 3500
F 0 "P8" H 1350 3250 50  0000 C CNN
F 1 "Digital" V 1350 3500 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x18_P2.54mm_Vertical" H 1300 2450 50  0001 C CNN
F 3 "" H 1300 2450 50  0000 C CNN
	1    1300 3500
	1    0    0    -1  
$EndComp
Text Label 1050 2800 2    50   ~ 0
PDIG_03
Text Label 1050 2900 2    50   ~ 0
PDIG_05
Text Label 1050 3000 2    50   ~ 0
PDIG_07
Text Label 1050 3100 2    50   ~ 0
PDIG_09
Text Label 1050 3200 2    50   ~ 0
PDIG_11
Text Label 1050 3300 2    50   ~ 0
PDIG_13
Text Label 1050 3400 2    50   ~ 0
PDIG_15
Text Label 1050 3500 2    50   ~ 0
PDIG_17
Text Label 1050 3600 2    50   ~ 0
PDIG_19
Text Label 1050 3700 2    50   ~ 0
PDIG_21
Text Label 1050 3800 2    50   ~ 0
PDIG_23
Text Label 1050 3900 2    50   ~ 0
PDIG_25
Text Label 1050 4000 2    50   ~ 0
PDIG_27
Text Label 1050 4100 2    50   ~ 0
PDIG_29
Text Label 1050 4200 2    50   ~ 0
PDIG_31
Text Label 1050 4300 2    50   ~ 0
PDIG_33
Text Label 1050 4400 2    50   ~ 0
PDIG_35
Text Label 1650 2700 0    50   ~ 0
PDIG_02
Text Label 1650 2800 0    50   ~ 0
PDIG_04
Text Label 1650 2900 0    50   ~ 0
PDIG_06
Text Label 1650 3000 0    50   ~ 0
PDIG_08
Text Label 1650 3100 0    50   ~ 0
PDIG_10
Text Label 1650 3200 0    50   ~ 0
PDIG_12
Text Label 1650 3300 0    50   ~ 0
PDIG_14
Text Label 1650 3400 0    50   ~ 0
PDIG_16
Text Label 1650 3500 0    50   ~ 0
PDIG_18
Text Label 1650 3600 0    50   ~ 0
PDIG_20
Text Label 1650 3700 0    50   ~ 0
PDIG_22
Text Label 1650 3800 0    50   ~ 0
PDIG_24
Text Label 1650 3900 0    50   ~ 0
PDIG_26
Text Label 1650 4000 0    50   ~ 0
PDIG_28
Text Label 1650 4100 0    50   ~ 0
PDIG_30
Text Label 1650 4200 0    50   ~ 0
PDIG_32
Text Label 1650 4300 0    50   ~ 0
PDIG_34
Text Label 1650 4400 0    50   ~ 0
PDIG_36
Wire Wire Line
	2800 2400 2450 2400
Wire Wire Line
	2800 2300 2450 2300
Wire Wire Line
	2800 2200 2450 2200
Wire Wire Line
	2800 2100 2450 2100
Wire Wire Line
	2800 2000 2450 2000
Text Label 2500 2000 0    50   ~ 0
PPOW_01
Text Label 2500 2100 0    50   ~ 0
PPOW_02
Text Label 2500 2200 0    50   ~ 0
PPOW_03
Text Label 2500 2300 0    50   ~ 0
PPOW_04
Text Label 2500 2400 0    50   ~ 0
PPOW_05
Wire Wire Line
	2800 2700 2450 2700
Wire Wire Line
	2800 2600 2450 2600
Wire Wire Line
	2800 2500 2450 2500
Text Label 2500 2500 0    50   ~ 0
PPOW_06
Text Label 2500 2600 0    50   ~ 0
PPOW_07
Text Label 2500 2700 0    50   ~ 0
PPOW_08
Wire Wire Line
	3700 2400 3350 2400
Wire Wire Line
	3700 2300 3350 2300
Wire Wire Line
	3700 2200 3350 2200
Wire Wire Line
	3700 2100 3350 2100
Wire Wire Line
	3700 2000 3350 2000
Text Label 3400 2000 0    50   ~ 0
PAN1_01
Text Label 3400 2100 0    50   ~ 0
PAN1_02
Text Label 3400 2200 0    50   ~ 0
PAN1_03
Text Label 3400 2300 0    50   ~ 0
PAN1_04
Text Label 3400 2400 0    50   ~ 0
PAN1_05
Wire Wire Line
	3700 2700 3350 2700
Wire Wire Line
	3700 2600 3350 2600
Wire Wire Line
	3700 2500 3350 2500
Text Label 3400 2500 0    50   ~ 0
PAN1_06
Text Label 3400 2600 0    50   ~ 0
PAN1_07
Text Label 3400 2700 0    50   ~ 0
PAN1_08
Wire Wire Line
	4550 2400 4200 2400
Wire Wire Line
	4550 2300 4200 2300
Wire Wire Line
	4550 2200 4200 2200
Wire Wire Line
	4550 2100 4200 2100
Wire Wire Line
	4550 2000 4200 2000
Text Label 4250 2000 0    50   ~ 0
PAN2_01
Text Label 4250 2100 0    50   ~ 0
PAN2_02
Text Label 4250 2200 0    50   ~ 0
PAN2_03
Text Label 4250 2300 0    50   ~ 0
PAN2_04
Text Label 4250 2400 0    50   ~ 0
PAN2_05
Wire Wire Line
	4550 2700 4200 2700
Wire Wire Line
	4550 2600 4200 2600
Wire Wire Line
	4550 2500 4200 2500
Text Label 4250 2500 0    50   ~ 0
PAN2_06
Text Label 4250 2600 0    50   ~ 0
PAN2_07
Text Label 4250 2700 0    50   ~ 0
PAN2_08
Wire Wire Line
	5400 2400 5050 2400
Wire Wire Line
	5400 2300 5050 2300
Wire Wire Line
	5400 2200 5050 2200
Wire Wire Line
	5400 2100 5050 2100
Wire Wire Line
	5400 2000 5050 2000
Text Label 5100 2000 0    50   ~ 0
PWM1_01
Text Label 5100 2100 0    50   ~ 0
PWM1_02
Text Label 5100 2200 0    50   ~ 0
PWM1_03
Text Label 5100 2300 0    50   ~ 0
PWM1_04
Text Label 5100 2400 0    50   ~ 0
PWM1_05
Wire Wire Line
	5400 2700 5050 2700
Wire Wire Line
	5400 2600 5050 2600
Wire Wire Line
	5400 2500 5050 2500
Text Label 5100 2500 0    50   ~ 0
PWM1_06
Text Label 5100 2600 0    50   ~ 0
PWM1_07
Text Label 5100 2700 0    50   ~ 0
PWM1_08
Wire Wire Line
	5400 2900 5050 2900
Wire Wire Line
	5400 2800 5050 2800
Text Label 5100 2800 0    50   ~ 0
PWM1_09
Text Label 5100 2900 0    50   ~ 0
PWM1_10
Wire Wire Line
	6250 2400 5900 2400
Wire Wire Line
	6250 2300 5900 2300
Wire Wire Line
	6250 2200 5900 2200
Wire Wire Line
	6250 2100 5900 2100
Wire Wire Line
	6250 2000 5900 2000
Text Label 5950 2000 0    50   ~ 0
PWM2_01
Text Label 5950 2100 0    50   ~ 0
PWM2_02
Text Label 5950 2200 0    50   ~ 0
PWM2_03
Text Label 5950 2300 0    50   ~ 0
PWM2_04
Text Label 5950 2400 0    50   ~ 0
PWM2_05
Wire Wire Line
	6250 2700 5900 2700
Wire Wire Line
	6250 2600 5900 2600
Wire Wire Line
	6250 2500 5900 2500
Text Label 5950 2500 0    50   ~ 0
PWM2_06
Text Label 5950 2600 0    50   ~ 0
PWM2_07
Text Label 5950 2700 0    50   ~ 0
PWM2_08
Wire Wire Line
	7150 2400 6800 2400
Wire Wire Line
	7150 2300 6800 2300
Wire Wire Line
	7150 2200 6800 2200
Wire Wire Line
	7150 2100 6800 2100
Wire Wire Line
	7150 2000 6800 2000
Text Label 6850 2000 0    50   ~ 0
COMM_01
Text Label 6850 2100 0    50   ~ 0
COMM_02
Text Label 6850 2200 0    50   ~ 0
COMM_03
Text Label 6850 2300 0    50   ~ 0
COMM_04
Text Label 6850 2400 0    50   ~ 0
COMM_05
Wire Wire Line
	7150 2700 6800 2700
Wire Wire Line
	7150 2600 6800 2600
Wire Wire Line
	7150 2500 6800 2500
Text Label 6850 2500 0    50   ~ 0
COMM_06
Text Label 6850 2600 0    50   ~ 0
COMM_07
Text Label 6850 2700 0    50   ~ 0
COMM_08
$Comp
L zxbus:ZXBUS Z1
U 1 1 5E3EDB25
P 9250 4000
F 0 "Z1" H 9600 7401 39  0000 C CNN
F 1 "ZXBUS" H 9600 7326 39  0000 C CNN
F 2 "zxbus:ZX_Bus_Connector" H 9250 4000 39  0001 C CNN
F 3 "" H 9250 4000 39  0001 C CNN
	1    9250 4000
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS08 U2
U 1 1 5E3F7BEA
P 3550 3950
F 0 "U2" H 3550 4275 50  0000 C CNN
F 1 "74LS08" H 3550 4184 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 3550 3950 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS08" H 3550 3950 50  0001 C CNN
	1    3550 3950
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS08 U2
U 2 1 5E405CFE
P 3550 4550
F 0 "U2" H 3550 4875 50  0000 C CNN
F 1 "74LS08" H 3550 4784 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 3550 4550 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS08" H 3550 4550 50  0001 C CNN
	2    3550 4550
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS138 U3
U 1 1 5E40B85A
P 5100 4250
F 0 "U3" H 5100 5031 50  0000 C CNN
F 1 "74LS138" H 5100 4940 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 5100 4250 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS138" H 5100 4250 50  0001 C CNN
	1    5100 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3950 4600 3950
Wire Wire Line
	4600 4050 4050 4050
Wire Wire Line
	4050 4050 4050 4550
Wire Wire Line
	4050 4550 3850 4550
Wire Wire Line
	10150 2800 10550 2800
Wire Wire Line
	10150 2900 10550 2900
Wire Wire Line
	10150 3000 10550 3000
Wire Wire Line
	10150 3100 10550 3100
Wire Wire Line
	3250 3850 2750 3850
Wire Wire Line
	3250 4050 2750 4050
Wire Wire Line
	3250 4450 2750 4450
Wire Wire Line
	3250 4650 2750 4650
Wire Wire Line
	10150 1900 10550 1900
Wire Wire Line
	3250 5250 2750 5250
Wire Wire Line
	4600 4450 4250 4450
Wire Wire Line
	4600 4550 4250 4550
Wire Wire Line
	3250 5050 2750 5050
Wire Wire Line
	8700 2800 9050 2800
Text Label 2850 5250 0    50   ~ 0
A0
Text Label 2850 3850 0    50   ~ 0
A7
Text Label 2850 4050 0    50   ~ 0
A6
Text Label 2850 4450 0    50   ~ 0
A5
Text Label 2850 4650 0    50   ~ 0
A4
Text Label 4350 4450 0    50   ~ 0
M1
Text Label 4350 4550 0    50   ~ 0
IOREQ
Text Label 8800 2800 0    50   ~ 0
WAIT
Text Label 10250 2800 0    50   ~ 0
A7
Text Label 10250 2900 0    50   ~ 0
A6
Text Label 10250 3000 0    50   ~ 0
A5
Text Label 10250 3100 0    50   ~ 0
A4
Text Label 10250 1900 0    50   ~ 0
A3
$Comp
L 74xx:74LS73 U1
U 3 1 5E56C056
P 1350 5150
F 0 "U1" V 983 5150 50  0000 C CNN
F 1 "74LS73A" V 1074 5150 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 1350 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS73" H 1350 5150 50  0001 C CNN
	3    1350 5150
	0    1    1    0   
$EndComp
$Comp
L 74xx:74LS08 U2
U 5 1 5E56DDBE
P 1350 5800
F 0 "U2" V 983 5800 50  0000 C CNN
F 1 "74LS08" V 1074 5800 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 1350 5800 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS08" H 1350 5800 50  0001 C CNN
	5    1350 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	8700 1500 9050 1500
Wire Wire Line
	9050 1300 8700 1300
Wire Wire Line
	9050 1400 8700 1400
Wire Wire Line
	9050 1600 8700 1600
Wire Wire Line
	9050 1700 8700 1700
Wire Wire Line
	9050 1900 8700 1900
Wire Wire Line
	9050 2500 8700 2500
Wire Wire Line
	9050 2400 8700 2400
Wire Wire Line
	9050 3100 8700 3100
Wire Wire Line
	10150 1100 10550 1100
Wire Wire Line
	10150 1600 10550 1600
Wire Wire Line
	10150 1700 10550 1700
Wire Wire Line
	10150 1800 10550 1800
Text Label 10250 1800 0    50   ~ 0
A2
Text Label 10250 1700 0    50   ~ 0
A1
Text Label 10250 1600 0    50   ~ 0
A0
$Comp
L power:GND #PWR0101
U 1 1 5E763FEF
P 10800 2200
F 0 "#PWR0101" H 10800 1950 50  0001 C CNN
F 1 "GND" H 10805 2027 50  0000 C CNN
F 2 "" H 10800 2200 50  0001 C CNN
F 3 "" H 10800 2200 50  0001 C CNN
	1    10800 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10800 1300 10800 1400
Wire Wire Line
	10150 1300 10800 1300
Wire Wire Line
	10150 2100 10800 2100
Connection ~ 10800 2100
Wire Wire Line
	10800 2100 10800 2200
Wire Wire Line
	10150 1400 10800 1400
Connection ~ 10800 1400
Wire Wire Line
	10800 1400 10800 2100
Text Label 10250 1100 0    50   ~ 0
9VCC
Text Label 8800 1300 0    50   ~ 0
D0
Text Label 8800 1400 0    50   ~ 0
D1
Text Label 8800 1500 0    50   ~ 0
D2
Text Label 8800 1600 0    50   ~ 0
D6
Text Label 8800 1700 0    50   ~ 0
D5
Text Label 8800 2400 0    50   ~ 0
IOREQ
Text Label 8800 2500 0    50   ~ 0
RD
Text Label 8800 3100 0    50   ~ 0
M1
Wire Wire Line
	9050 1000 8700 1000
Text Label 8800 1000 0    50   ~ 0
D7
Text Label 2850 5050 0    50   ~ 0
EN_PERI
Wire Wire Line
	8950 4000 9750 4000
Wire Wire Line
	9750 3800 8950 3800
Wire Wire Line
	9750 3900 8950 3900
Wire Wire Line
	9750 4100 8950 4100
Wire Wire Line
	9750 4200 8950 4200
Wire Wire Line
	9750 4300 8950 4300
Wire Wire Line
	9750 4400 8950 4400
Text Label 9050 3800 0    50   ~ 0
D0
Text Label 9050 3900 0    50   ~ 0
D1
Text Label 9050 4000 0    50   ~ 0
D2
Text Label 9050 4400 0    50   ~ 0
D6
Text Label 9050 4300 0    50   ~ 0
D5
Text Label 9050 4100 0    50   ~ 0
D3
Text Label 9050 4200 0    50   ~ 0
D4
Wire Wire Line
	8950 4600 9750 4600
Wire Wire Line
	8950 4700 9750 4700
Text Label 9050 4700 0    50   ~ 0
A1
Text Label 9050 4600 0    50   ~ 0
A0
Wire Wire Line
	9950 4500 10750 4500
Wire Wire Line
	8950 4800 9750 4800
Text Label 9050 4800 0    50   ~ 0
A2
Wire Wire Line
	9950 4900 10750 4900
Text Label 10050 4900 0    50   ~ 0
9VCC
Text Label 9400 3800 0    50   ~ 0
PDIG_08
Text Label 9400 3900 0    50   ~ 0
PDIG_07
Text Label 9400 4000 0    50   ~ 0
PDIG_10
Text Label 9400 4100 0    50   ~ 0
PDIG_09
Text Label 9400 4200 0    50   ~ 0
PDIG_12
Text Label 9400 4300 0    50   ~ 0
PDIG_11
Text Label 9400 4400 0    50   ~ 0
PDIG_14
Wire Wire Line
	9750 4500 8950 4500
Text Label 9050 4500 0    50   ~ 0
D7
Text Label 9400 4500 0    50   ~ 0
PDIG_13
Text Label 9400 4600 0    50   ~ 0
PDIG_33
Text Label 9400 4700 0    50   ~ 0
PDIG_34
Text Label 9400 4800 0    50   ~ 0
PDIG_31
Wire Wire Line
	8950 4900 9750 4900
Text Label 9050 4900 0    50   ~ 0
A3
Text Label 9400 4900 0    50   ~ 0
PDIG_32
Wire Wire Line
	8950 5000 9750 5000
Text Label 9050 5000 0    50   ~ 0
A4
Text Label 9400 5000 0    50   ~ 0
PDIG_29
Text Label 10400 4500 0    50   ~ 0
PWM2_06
Text Label 10350 4900 0    50   ~ 0
PPOW_08
Text Label 10350 5000 0    50   ~ 0
PPOW_07
Text Label 10350 5100 0    50   ~ 0
PPOW_06
Text Label 10350 5200 0    50   ~ 0
PPOW_05
Wire Wire Line
	10250 5200 10750 5200
$Comp
L power:GND #PWR0104
U 1 1 5ED63248
P 9950 5200
F 0 "#PWR0104" H 9950 4950 50  0001 C CNN
F 1 "GND" H 9955 5027 50  0000 C CNN
F 2 "" H 9950 5200 50  0001 C CNN
F 3 "" H 9950 5200 50  0001 C CNN
	1    9950 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 5000 9950 5100
Wire Wire Line
	9950 5000 10750 5000
Wire Wire Line
	9950 5100 10750 5100
Connection ~ 9950 5100
Wire Wire Line
	9950 5100 9950 5200
$Comp
L power:VCC #PWR0105
U 1 1 5ED9352F
P 10250 5200
F 0 "#PWR0105" H 10250 5050 50  0001 C CNN
F 1 "VCC" V 10268 5327 50  0000 L CNN
F 2 "" H 10250 5200 50  0001 C CNN
F 3 "" H 10250 5200 50  0001 C CNN
	1    10250 5200
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5ED94E33
P 850 7050
F 0 "C1" H 965 7096 50  0000 L CNN
F 1 "C" H 965 7005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 888 6900 50  0001 C CNN
F 3 "~" H 850 7050 50  0001 C CNN
	1    850  7050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5ED967AF
P 1300 7050
F 0 "C2" H 1415 7096 50  0000 L CNN
F 1 "C" H 1415 7005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 1338 6900 50  0001 C CNN
F 3 "~" H 1300 7050 50  0001 C CNN
	1    1300 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5EDADF63
P 1750 7050
F 0 "C3" H 1865 7096 50  0000 L CNN
F 1 "C" H 1865 7005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 1788 6900 50  0001 C CNN
F 3 "~" H 1750 7050 50  0001 C CNN
	1    1750 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 6900 1750 6800
Wire Wire Line
	850  6800 850  6900
Wire Wire Line
	1750 6800 1550 6800
Wire Wire Line
	1300 6900 1300 6800
Connection ~ 1300 6800
Wire Wire Line
	1300 6800 850  6800
Wire Wire Line
	1750 7200 1750 7350
Wire Wire Line
	1750 7350 1550 7350
Wire Wire Line
	850  7350 850  7200
Wire Wire Line
	1300 7200 1300 7350
Connection ~ 1300 7350
Wire Wire Line
	1300 7350 850  7350
$Comp
L power:VCC #PWR0106
U 1 1 5EE8C2D2
P 1550 6700
F 0 "#PWR0106" H 1550 6550 50  0001 C CNN
F 1 "VCC" H 1567 6873 50  0000 C CNN
F 2 "" H 1550 6700 50  0001 C CNN
F 3 "" H 1550 6700 50  0001 C CNN
	1    1550 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5EE8CFC8
P 1550 7450
F 0 "#PWR0107" H 1550 7200 50  0001 C CNN
F 1 "GND" H 1555 7277 50  0000 C CNN
F 2 "" H 1550 7450 50  0001 C CNN
F 3 "" H 1550 7450 50  0001 C CNN
	1    1550 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 6700 1550 6800
Wire Wire Line
	1550 7350 1550 7450
Wire Wire Line
	950  5150 750  5150
Wire Wire Line
	750  5150 750  5800
Wire Wire Line
	1750 5150 1950 5150
Wire Wire Line
	1950 5150 1950 5800
Wire Wire Line
	850  5800 750  5800
Connection ~ 750  5800
Wire Wire Line
	750  5800 750  6150
Wire Wire Line
	1850 5800 1950 5800
Wire Wire Line
	1950 6150 1950 5800
Connection ~ 1950 5800
$Comp
L power:GND #PWR0108
U 1 1 5EF5C306
P 750 6150
F 0 "#PWR0108" H 750 5900 50  0001 C CNN
F 1 "GND" H 755 5977 50  0000 C CNN
F 2 "" H 750 6150 50  0001 C CNN
F 3 "" H 750 6150 50  0001 C CNN
	1    750  6150
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0109
U 1 1 5EF76840
P 1950 6150
F 0 "#PWR0109" H 1950 6000 50  0001 C CNN
F 1 "VCC" H 1967 6323 50  0000 C CNN
F 2 "" H 1950 6150 50  0001 C CNN
F 3 "" H 1950 6150 50  0001 C CNN
	1    1950 6150
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR0110
U 1 1 5EF9154D
P 5100 3350
F 0 "#PWR0110" H 5100 3200 50  0001 C CNN
F 1 "VCC" H 5117 3523 50  0000 C CNN
F 2 "" H 5100 3350 50  0001 C CNN
F 3 "" H 5100 3350 50  0001 C CNN
	1    5100 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3350 5100 3650
$Comp
L power:GND #PWR0111
U 1 1 5F03D7B1
P 5100 5100
F 0 "#PWR0111" H 5100 4850 50  0001 C CNN
F 1 "GND" H 5105 4927 50  0000 C CNN
F 2 "" H 5100 5100 50  0001 C CNN
F 3 "" H 5100 5100 50  0001 C CNN
	1    5100 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 4950 5100 5100
Wire Wire Line
	7300 5350 7400 5350
$Comp
L power:VCC #PWR0102
U 1 1 5E90369C
P 7300 5350
F 0 "#PWR0102" H 7300 5200 50  0001 C CNN
F 1 "VCC" V 7318 5477 50  0000 L CNN
F 2 "" H 7300 5350 50  0001 C CNN
F 3 "" H 7300 5350 50  0001 C CNN
	1    7300 5350
	0    -1   -1   0   
$EndComp
Text Label 7850 5900 0    50   ~ 0
RELEASE
Wire Wire Line
	7700 5900 8550 5900
Wire Wire Line
	7700 5750 7700 5900
Text Label 8100 5550 0    50   ~ 0
WAIT
Wire Wire Line
	8000 5550 8550 5550
$Comp
L 74xx:74LS73 U1
U 2 1 5E876F76
P 7700 5450
F 0 "U1" H 7700 5817 50  0000 C CNN
F 1 "74LS73A" H 7700 5726 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 7700 5450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS73" H 7700 5450 50  0001 C CNN
	2    7700 5450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS138 U4
U 1 1 5E56B10B
P 5100 6550
F 0 "U4" H 5100 7331 50  0000 C CNN
F 1 "74LS138" H 5100 7240 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket" H 5100 6550 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS138" H 5100 6550 50  0001 C CNN
	1    5100 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 6850 4250 6850
Wire Wire Line
	4600 6350 4250 6350
Wire Wire Line
	4600 6950 4250 6950
Text Label 4350 6350 0    50   ~ 0
A15
Text Label 2900 6150 0    50   ~ 0
M1
Text Label 4350 6950 0    50   ~ 0
MREQ
$Comp
L power:VCC #PWR0103
U 1 1 5E56B11B
P 5100 5650
F 0 "#PWR0103" H 5100 5500 50  0001 C CNN
F 1 "VCC" H 5117 5823 50  0000 C CNN
F 2 "" H 5100 5650 50  0001 C CNN
F 3 "" H 5100 5650 50  0001 C CNN
	1    5100 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 5650 5100 5950
$Comp
L power:GND #PWR0112
U 1 1 5E56B122
P 5100 7400
F 0 "#PWR0112" H 5100 7150 50  0001 C CNN
F 1 "GND" H 5105 7227 50  0000 C CNN
F 2 "" H 5100 7400 50  0001 C CNN
F 3 "" H 5100 7400 50  0001 C CNN
	1    5100 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 7250 5100 7400
Wire Wire Line
	3150 6050 3250 6050
$Comp
L power:VCC #PWR0113
U 1 1 5E60E79F
P 3150 6050
F 0 "#PWR0113" H 3150 5900 50  0001 C CNN
F 1 "VCC" V 3168 6177 50  0000 L CNN
F 2 "" H 3150 6050 50  0001 C CNN
F 3 "" H 3150 6050 50  0001 C CNN
	1    3150 6050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9050 2100 8700 2100
Text Label 8800 2100 0    50   ~ 0
NMI
Wire Wire Line
	2850 6750 3550 6750
Wire Wire Line
	3550 6750 3550 6450
Text Label 2950 6750 0    50   ~ 0
EN_ROM
Text Label 4350 6850 0    50   ~ 0
A14
Text Label 5850 7050 0    50   ~ 0
INT_ROM
Wire Wire Line
	5800 7050 6200 7050
Wire Wire Line
	5600 6650 5800 6650
Wire Wire Line
	5800 7050 5800 6650
Connection ~ 5800 6650
$Comp
L 74xx:74LS08 U2
U 3 1 5E9F45C6
P 6650 5450
F 0 "U2" H 6650 5775 50  0000 C CNN
F 1 "74LS08" H 6650 5684 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 6650 5450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS08" H 6650 5450 50  0001 C CNN
	3    6650 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 5450 6950 5450
Wire Wire Line
	6350 4650 6350 5350
Wire Wire Line
	5800 6650 6350 6650
Wire Wire Line
	2850 6150 3250 6150
$Comp
L power:GND #PWR0114
U 1 1 5EFF262B
P 7250 5650
F 0 "#PWR0114" H 7250 5400 50  0001 C CNN
F 1 "GND" H 7255 5477 50  0000 C CNN
F 2 "" H 7250 5650 50  0001 C CNN
F 3 "" H 7250 5650 50  0001 C CNN
	1    7250 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5EFF369F
P 3100 6350
F 0 "#PWR0115" H 3100 6100 50  0001 C CNN
F 1 "GND" H 3105 6177 50  0000 C CNN
F 2 "" H 3100 6350 50  0001 C CNN
F 3 "" H 3100 6350 50  0001 C CNN
	1    3100 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6250 3100 6350
Wire Wire Line
	3100 6250 3250 6250
Wire Wire Line
	7250 5550 7250 5650
Wire Wire Line
	7250 5550 7400 5550
Wire Wire Line
	9050 3300 8700 3300
Text Label 8800 3300 0    50   ~ 0
A8
Wire Wire Line
	9050 3400 8700 3400
Text Label 8800 3400 0    50   ~ 0
A10
Wire Wire Line
	10550 3500 10150 3500
Text Label 10250 3500 0    50   ~ 0
A11
Wire Wire Line
	10550 3400 10150 3400
Text Label 10250 3400 0    50   ~ 0
A9
Wire Wire Line
	10500 900  10150 900 
Text Label 10250 900  0    50   ~ 0
A12
Wire Wire Line
	9050 900  8700 900 
Text Label 8800 900  0    50   ~ 0
A13
Wire Wire Line
	9050 800  8700 800 
Text Label 8800 800  0    50   ~ 0
A15
Wire Wire Line
	10500 800  10150 800 
Text Label 10250 800  0    50   ~ 0
A14
Wire Wire Line
	10150 3200 10550 3200
Wire Wire Line
	9050 1100 8700 1100
Wire Wire Line
	10150 2200 10550 2200
Text Label 8800 1100 0    50   ~ 0
R_128_A
Text Label 10150 2200 0    50   ~ 0
R_128_B
Text Label 10200 3200 0    50   ~ 0
R_48K
$Comp
L Diode:1N4148 D1
U 1 1 5F2A2AAF
P 8050 3850
F 0 "D1" H 8050 3634 50  0000 C CNN
F 1 "1N4148" H 8050 3725 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 8050 3675 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 8050 3850 50  0001 C CNN
	1    8050 3850
	-1   0    0    1   
$EndComp
$Comp
L Diode:1N4148 D2
U 1 1 5F2A455D
P 8050 4200
F 0 "D2" H 8050 3984 50  0000 C CNN
F 1 "1N4148" H 8050 4075 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 8050 4025 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 8050 4200 50  0001 C CNN
	1    8050 4200
	-1   0    0    1   
$EndComp
Text Label 8300 3850 0    50   ~ 0
R_128_A
Text Label 8300 4200 0    50   ~ 0
R_128_B
Wire Wire Line
	8200 3850 8700 3850
Wire Wire Line
	8200 4200 8700 4200
Wire Wire Line
	7900 3850 7750 3850
Wire Wire Line
	7750 3850 7750 4000
Wire Wire Line
	7750 4200 7900 4200
Connection ~ 7750 4000
Wire Wire Line
	7750 4000 7750 4200
Text Label 8300 4450 0    50   ~ 0
R_48K
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5F538E28
P 7250 3800
F 0 "J1" V 7358 3612 50  0000 R CNN
F 1 "Conn_01x02_Male" H 7358 3890 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7250 3800 50  0001 C CNN
F 3 "~" H 7250 3800 50  0001 C CNN
	1    7250 3800
	0    -1   1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5F55D772
P 7250 4250
F 0 "J2" H 7358 4431 50  0000 C CNN
F 1 "Conn_01x02_Male" V 7358 4340 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7250 4250 50  0001 C CNN
F 3 "~" H 7250 4250 50  0001 C CNN
	1    7250 4250
	0    -1   1    0   
$EndComp
Wire Wire Line
	7350 4450 8700 4450
Wire Wire Line
	7350 4000 7750 4000
Wire Wire Line
	7250 4000 6950 4000
Wire Wire Line
	6950 4450 7250 4450
Wire Wire Line
	6350 3600 5900 3600
Text Label 5950 3600 0    50   ~ 0
ROM_DIS
Wire Wire Line
	10150 2700 10550 2700
Text Label 10250 2700 0    50   ~ 0
RESET
Wire Wire Line
	4600 6750 3550 6750
Wire Wire Line
	8950 5300 9750 5300
Wire Wire Line
	9750 5100 8950 5100
Wire Wire Line
	9750 5200 8950 5200
Wire Wire Line
	9750 5400 8950 5400
Wire Wire Line
	9750 5500 8950 5500
Wire Wire Line
	9750 5600 8950 5600
Wire Wire Line
	9750 5700 8950 5700
Text Label 9050 5100 0    50   ~ 0
A5
Text Label 9050 5200 0    50   ~ 0
A6
Text Label 9050 5300 0    50   ~ 0
A7
Text Label 9050 5700 0    50   ~ 0
A11
Text Label 9050 5600 0    50   ~ 0
A10
Text Label 9050 5400 0    50   ~ 0
A8
Text Label 9050 5500 0    50   ~ 0
A9
Wire Wire Line
	8950 5900 9750 5900
Text Label 9050 5900 0    50   ~ 0
A13
Wire Wire Line
	9950 4600 10750 4600
Text Label 10050 4600 0    50   ~ 0
INT_ROM
Text Label 9400 5100 0    50   ~ 0
PDIG_30
Text Label 9400 5200 0    50   ~ 0
PDIG_27
Text Label 9400 5300 0    50   ~ 0
PDIG_28
Text Label 9400 5400 0    50   ~ 0
PDIG_20
Text Label 9400 5500 0    50   ~ 0
PDIG_19
Text Label 9400 5600 0    50   ~ 0
PDIG_22
Text Label 9400 5700 0    50   ~ 0
PDIG_21
Wire Wire Line
	9750 5800 8950 5800
Text Label 9050 5800 0    50   ~ 0
A12
Text Label 9400 5800 0    50   ~ 0
PDIG_24
Text Label 9400 5900 0    50   ~ 0
PDIG_23
Wire Wire Line
	8950 6000 9750 6000
Text Label 9050 6000 0    50   ~ 0
RD
Text Label 9400 6000 0    50   ~ 0
PDIG_26
Wire Wire Line
	10150 6750 10950 6750
Text Label 10250 6750 0    50   ~ 0
DIS_PER
Text Label 10600 6750 0    50   ~ 0
PDIG_28
Text Label 10400 4600 0    50   ~ 0
PWM2_05
Text Label 10050 4500 0    50   ~ 0
INT_PER
Wire Wire Line
	9950 3800 10750 3800
Text Label 10050 3800 0    50   ~ 0
EN_PERI
Text Label 10400 3800 0    50   ~ 0
PDIG_17
Wire Wire Line
	9950 3900 10750 3900
Text Label 10050 3900 0    50   ~ 0
EN_ROM
Text Label 10400 3900 0    50   ~ 0
PDIG_18
Wire Wire Line
	9950 4000 10750 4000
Text Label 10050 4000 0    50   ~ 0
NMI
Text Label 10400 4000 0    50   ~ 0
PDIG_15
Wire Wire Line
	9950 4200 10750 4200
Text Label 10050 4200 0    50   ~ 0
RESET
Text Label 10400 4200 0    50   ~ 0
PDIG_25
Text Label 3900 6050 0    50   ~ 0
ROM_DIS
$Comp
L 74xx:74LS73 U1
U 1 1 6001CA07
P 3550 6150
F 0 "U1" H 3550 6517 50  0000 C CNN
F 1 "74LS73A" H 3550 6426 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 3550 6150 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS73" H 3550 6150 50  0001 C CNN
	1    3550 6150
	1    0    0    -1  
$EndComp
Connection ~ 3550 6750
Wire Wire Line
	3850 6250 4600 6250
Wire Wire Line
	3850 6050 4250 6050
Wire Wire Line
	9050 3200 8700 3200
Text Label 8800 3200 0    50   ~ 0
RFSH
Wire Wire Line
	4600 6450 4250 6450
Text Label 4350 6450 0    50   ~ 0
RFSH
Wire Wire Line
	6350 5550 6350 6650
Wire Wire Line
	9950 4100 10750 4100
Text Label 10050 4100 0    50   ~ 0
RELEASE
Text Label 10400 4100 0    50   ~ 0
PDIG_16
$Comp
L Device:C C4
U 1 1 5E4F35B9
P 2200 7050
F 0 "C4" H 2315 7096 50  0000 L CNN
F 1 "C" H 2315 7005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 2238 6900 50  0001 C CNN
F 3 "~" H 2200 7050 50  0001 C CNN
	1    2200 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 6900 2200 6800
Wire Wire Line
	2200 7200 2200 7350
Connection ~ 1550 6800
Wire Wire Line
	1550 6800 1300 6800
Connection ~ 1550 7350
Wire Wire Line
	1550 7350 1300 7350
Wire Wire Line
	1750 6800 2200 6800
Connection ~ 1750 6800
Wire Wire Line
	2200 7350 1750 7350
Connection ~ 1750 7350
$Comp
L 74xx:74LS08 U2
U 4 1 5E3AE2DD
P 3550 5150
F 0 "U2" H 3550 5475 50  0000 C CNN
F 1 "74LS08" H 3550 5384 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 3550 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS08" H 3550 5150 50  0001 C CNN
	4    3550 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4650 4250 4650
Text Label 4350 4650 0    50   ~ 0
A3
Wire Wire Line
	4600 4150 4150 4150
Wire Wire Line
	4150 4150 4150 5150
Wire Wire Line
	4150 5150 3850 5150
Wire Wire Line
	9050 2000 8700 2000
Text Label 8800 2000 0    50   ~ 0
INT
Wire Wire Line
	9950 4300 10750 4300
Text Label 10050 4300 0    50   ~ 0
INT
Text Label 10400 4300 0    50   ~ 0
PWM2_03
$Comp
L Transistor_BJT:2N3904 Q1
U 1 1 5E3D61B3
P 6850 3600
F 0 "Q1" H 7040 3646 50  0000 L CNN
F 1 "2N3904" H 7040 3555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline_Wide" H 7050 3525 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 6850 3600 50  0001 L CNN
	1    6850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 4000 6950 4450
Wire Wire Line
	6950 3800 6950 4000
Connection ~ 6950 4000
$Comp
L Device:R R1
U 1 1 5E42F1AA
P 6500 3600
F 0 "R1" V 6293 3600 50  0000 C CNN
F 1 "R" V 6384 3600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6430 3600 50  0001 C CNN
F 3 "~" H 6500 3600 50  0001 C CNN
	1    6500 3600
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR0116
U 1 1 5E487EAC
P 7100 3250
F 0 "#PWR0116" H 7100 3100 50  0001 C CNN
F 1 "VCC" H 7117 3423 50  0000 C CNN
F 2 "" H 7100 3250 50  0001 C CNN
F 3 "" H 7100 3250 50  0001 C CNN
	1    7100 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 3250 6950 3250
Wire Wire Line
	6950 3250 6950 3400
Wire Wire Line
	9050 2300 8700 2300
Text Label 8800 2300 0    50   ~ 0
MREQ
Wire Wire Line
	5800 4650 6350 4650
Connection ~ 5800 4650
Wire Wire Line
	5800 4250 5800 4650
Wire Wire Line
	5600 4650 5800 4650
Wire Wire Line
	5800 4250 6200 4250
Text Label 5850 4250 0    50   ~ 0
INT_PER
Text Label 8800 1900 0    50   ~ 0
D4
Text Label 8800 1800 0    50   ~ 0
D3
Wire Wire Line
	9050 1800 8700 1800
$EndSCHEMATC
