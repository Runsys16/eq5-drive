EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Driver_Motor:Pololu_Breakout_A4988 A1
U 1 1 5E01E61A
P 7600 1950
F 0 "A1" H 8150 1250 50  0000 C CNN
F 1 "Pololu_Breakout_A4988" H 8600 1400 50  0000 C CNN
F 2 "Modules:Pololu_Breakout-16_15.2x20.3mm" H 7875 1200 50  0001 L CNN
F 3 "https://www.pololu.com/product/2980/pictures" H 7700 1650 50  0001 C CNN
	1    7600 1950
	1    0    0    -1  
$EndComp
$Comp
L Driver_Motor:Pololu_Breakout_A4988 A2
U 1 1 5E01EDD5
P 7650 4700
F 0 "A2" H 8000 3850 50  0000 C CNN
F 1 "Pololu_Breakout_A4988" H 7650 3600 50  0000 C CNN
F 2 "Modules:Pololu_Breakout-16_15.2x20.3mm" H 7925 3950 50  0001 L CNN
F 3 "https://www.pololu.com/product/2980/pictures" H 7750 4400 50  0001 C CNN
	1    7650 4700
	1    0    0    -1  
$EndComp
Entry Wire Line
	1750 3400 1850 3500
Entry Wire Line
	1750 3500 1850 3600
Entry Wire Line
	1750 3700 1850 3800
Entry Wire Line
	1750 3900 1850 4000
Entry Wire Line
	1750 4000 1850 4100
Entry Wire Line
	1750 3800 1850 3900
Wire Wire Line
	2100 3500 1850 3500
Wire Wire Line
	1850 3600 2100 3600
Wire Wire Line
	2100 3700 1850 3700
Wire Wire Line
	1850 3800 2100 3800
Wire Wire Line
	2100 3900 1850 3900
Wire Wire Line
	1850 4000 2100 4000
Wire Wire Line
	2100 4100 1850 4100
Entry Wire Line
	1750 3600 1850 3700
Text Label 1850 3500 0    50   ~ 0
A9
Text Label 1850 3600 0    50   ~ 0
A10
Text Label 1850 3700 0    50   ~ 0
A11
Text Label 1850 3800 0    50   ~ 0
A12
Text Label 1850 3900 0    50   ~ 0
A13
Text Label 1850 4000 0    50   ~ 0
A14
Text Label 1850 4100 0    50   ~ 0
A15
Wire Bus Line
	1750 900  4800 900 
Entry Wire Line
	6900 1750 7000 1850
Entry Wire Line
	6900 1850 7000 1950
Entry Wire Line
	7000 2050 6900 1950
Entry Wire Line
	7000 2250 6900 2150
Entry Wire Line
	7000 2350 6900 2250
Entry Wire Line
	7000 2450 6900 2350
Entry Wire Line
	7000 1550 6900 1450
Wire Wire Line
	7000 1550 7200 1550
Wire Wire Line
	7200 2050 7000 2050
Wire Wire Line
	7000 2450 7200 2450
Text Label 7000 1850 0    50   ~ 0
A0
Text Label 7000 2250 0    50   ~ 0
A1
Text Label 7000 2350 0    50   ~ 0
A2
Text Label 7000 2450 0    50   ~ 0
A3
Text Label 7000 1950 0    50   ~ 0
A5
Text Label 7000 2050 0    50   ~ 0
A6
Text Label 7000 1550 0    50   ~ 0
A4
Entry Wire Line
	4700 1800 4800 1700
Entry Wire Line
	4700 1900 4800 1800
Entry Wire Line
	4700 2000 4800 1900
Entry Wire Line
	4700 2100 4800 2000
Entry Wire Line
	4700 2200 4800 2100
Entry Wire Line
	4700 2300 4800 2200
Entry Wire Line
	4700 2500 4800 2400
Wire Wire Line
	4425 1800 4700 1800
Wire Wire Line
	4425 1900 4700 1900
Entry Wire Line
	7000 4300 6900 4200
Entry Wire Line
	7000 4600 6900 4500
Entry Wire Line
	7000 4700 6900 4600
Entry Wire Line
	7000 4800 6900 4700
Entry Wire Line
	7000 5000 6900 4900
Entry Wire Line
	7000 5100 6900 5000
Entry Wire Line
	7000 5200 6900 5100
Text Label 4600 1800 0    50   ~ 0
13
Text Label 4600 1900 0    50   ~ 0
12
Text Label 4600 2000 0    50   ~ 0
11
Text Label 4600 2100 0    50   ~ 0
10
Text Label 4600 2200 0    50   ~ 0
9
Text Label 4600 2300 0    50   ~ 0
8
Text Label 4625 2500 0    50   ~ 0
7
Text Label 7050 4300 0    50   ~ 0
6
Text Label 7050 4600 0    50   ~ 0
2
Text Label 7050 5000 0    50   ~ 0
3
Text Label 7050 5100 0    50   ~ 0
4
Text Label 7050 5200 0    50   ~ 0
5
Text Label 7050 4700 0    50   ~ 0
7
Text Label 7050 4800 0    50   ~ 0
8
$Comp
L power:GND #PWR02
U 1 1 5E037BD3
P 1550 2200
F 0 "#PWR02" H 1550 1950 50  0001 C CNN
F 1 "GND" V 1555 2072 50  0000 R CNN
F 2 "" H 1550 2200 50  0001 C CNN
F 3 "" H 1550 2200 50  0001 C CNN
	1    1550 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 2200 2100 2100
$Comp
L power:+5V #PWR01
U 1 1 5E03AE92
P 1550 2000
F 0 "#PWR01" H 1550 1850 50  0001 C CNN
F 1 "+5V" V 1565 2128 50  0000 L CNN
F 2 "" H 1550 2000 50  0001 C CNN
F 3 "" H 1550 2000 50  0001 C CNN
	1    1550 2000
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR03
U 1 1 5E03C774
P 1550 2300
F 0 "#PWR03" H 1550 2150 50  0001 C CNN
F 1 "+12V" V 1565 2428 50  0000 L CNN
F 2 "" H 1550 2300 50  0001 C CNN
F 3 "" H 1550 2300 50  0001 C CNN
	1    1550 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1550 2000 2100 2000
Wire Wire Line
	1550 2300 2100 2300
Wire Wire Line
	1550 2200 2100 2200
$Comp
L power:GND #PWR04
U 1 1 5E0495F7
P 4450 1700
F 0 "#PWR04" H 4450 1450 50  0001 C CNN
F 1 "GND" V 4455 1572 50  0000 R CNN
F 2 "" H 4450 1700 50  0001 C CNN
F 3 "" H 4450 1700 50  0001 C CNN
	1    4450 1700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5E049AD7
P 7600 2750
F 0 "#PWR08" H 7600 2500 50  0001 C CNN
F 1 "GND" H 7605 2577 50  0000 C CNN
F 2 "" H 7600 2750 50  0001 C CNN
F 3 "" H 7600 2750 50  0001 C CNN
	1    7600 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 2750 7800 2750
Connection ~ 7600 2750
$Comp
L power:+12V #PWR012
U 1 1 5E04B5A1
P 8500 1200
F 0 "#PWR012" H 8500 1050 50  0001 C CNN
F 1 "+12V" V 8515 1328 50  0000 L CNN
F 2 "" H 8500 1200 50  0001 C CNN
F 3 "" H 8500 1200 50  0001 C CNN
	1    8500 1200
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 5E04BBAE
P 7400 1200
F 0 "#PWR05" H 7400 1050 50  0001 C CNN
F 1 "+5V" V 7415 1328 50  0000 L CNN
F 2 "" H 7400 1200 50  0001 C CNN
F 3 "" H 7400 1200 50  0001 C CNN
	1    7400 1200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7400 1200 7500 1200
Wire Wire Line
	7600 1200 7600 1250
Wire Wire Line
	7800 1250 7800 1200
$Comp
L power:+12V #PWR010
U 1 1 5E04FDA1
P 8700 3950
F 0 "#PWR010" H 8700 3800 50  0001 C CNN
F 1 "+12V" V 8715 4078 50  0000 L CNN
F 2 "" H 8700 3950 50  0001 C CNN
F 3 "" H 8700 3950 50  0001 C CNN
	1    8700 3950
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 5E04FDA7
P 7450 3950
F 0 "#PWR06" H 7450 3800 50  0001 C CNN
F 1 "+5V" V 7465 4078 50  0000 L CNN
F 2 "" H 7450 3950 50  0001 C CNN
F 3 "" H 7450 3950 50  0001 C CNN
	1    7450 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7450 3950 7550 3950
Wire Wire Line
	7650 3950 7650 4000
Wire Wire Line
	7850 4000 7850 3950
$Comp
L power:GND #PWR09
U 1 1 5E053233
P 7650 5500
F 0 "#PWR09" H 7650 5250 50  0001 C CNN
F 1 "GND" H 7655 5327 50  0000 C CNN
F 2 "" H 7650 5500 50  0001 C CNN
F 3 "" H 7650 5500 50  0001 C CNN
	1    7650 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 5500 7850 5500
Connection ~ 7650 5500
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5E0571A5
P 9100 2050
F 0 "J1" H 9072 1932 50  0000 R CNN
F 1 "Conn_01x04_Male" H 9072 2023 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S4B-XH-A-1_1x04_P2.50mm_Horizontal" H 9100 2050 50  0001 C CNN
F 3 "~" H 9100 2050 50  0001 C CNN
	1    9100 2050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5E057AE7
P 9150 4800
F 0 "J2" H 9122 4682 50  0000 R CNN
F 1 "Conn_01x04_Male" H 9122 4773 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S4B-XH-A-1_1x04_P2.50mm_Horizontal" H 9150 4800 50  0001 C CNN
F 3 "~" H 9150 4800 50  0001 C CNN
	1    9150 4800
	-1   0    0    1   
$EndComp
Wire Wire Line
	8950 4600 8150 4600
Wire Wire Line
	8150 4700 8950 4700
Wire Wire Line
	8950 4800 8150 4800
Wire Wire Line
	8150 4900 8950 4900
Wire Wire Line
	8100 1850 8900 1850
Wire Wire Line
	8900 1950 8100 1950
Wire Wire Line
	8100 2050 8900 2050
Wire Wire Line
	8900 2150 8100 2150
$Comp
L Device:CP_Small C2
U 1 1 5E064E95
P 8300 1400
F 0 "C2" H 8388 1446 50  0000 L CNN
F 1 "100u" H 8388 1355 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 8300 1400 50  0001 C CNN
F 3 "~" H 8300 1400 50  0001 C CNN
	1    8300 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5E065AA4
P 8300 1500
F 0 "#PWR011" H 8300 1250 50  0001 C CNN
F 1 "GND" H 8305 1327 50  0000 C CNN
F 2 "" H 8300 1500 50  0001 C CNN
F 3 "" H 8300 1500 50  0001 C CNN
	1    8300 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 1300 8300 1200
Wire Wire Line
	8300 1200 8500 1200
Wire Wire Line
	7800 1200 8300 1200
Connection ~ 8300 1200
$Comp
L Device:CP_Small C1
U 1 1 5E0693B5
P 7500 950
F 0 "C1" H 7412 904 50  0000 R CNN
F 1 "100n" H 7850 850 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 7500 950 50  0001 C CNN
F 3 "~" H 7500 950 50  0001 C CNN
	1    7500 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	7500 1050 7500 1200
Connection ~ 7500 1200
Wire Wire Line
	7500 1200 7600 1200
$Comp
L power:GND #PWR07
U 1 1 5E06B6CB
P 7500 850
F 0 "#PWR07" H 7500 600 50  0001 C CNN
F 1 "GND" H 7505 677 50  0000 C CNN
F 2 "" H 7500 850 50  0001 C CNN
F 3 "" H 7500 850 50  0001 C CNN
	1    7500 850 
	-1   0    0    1   
$EndComp
$Comp
L Device:CP_Small C3
U 1 1 5E073C8D
P 7550 3750
F 0 "C3" H 7462 3704 50  0000 R CNN
F 1 "100n" H 7900 3650 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D5.1mm_W3.2mm_P5.00mm" H 7550 3750 50  0001 C CNN
F 3 "~" H 7550 3750 50  0001 C CNN
	1    7550 3750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E073C93
P 7550 3650
F 0 "#PWR0101" H 7550 3400 50  0001 C CNN
F 1 "GND" H 7555 3477 50  0000 C CNN
F 2 "" H 7550 3650 50  0001 C CNN
F 3 "" H 7550 3650 50  0001 C CNN
	1    7550 3650
	-1   0    0    1   
$EndComp
$Comp
L Device:CP_Small C4
U 1 1 5E076114
P 8400 4150
F 0 "C4" H 8488 4196 50  0000 L CNN
F 1 "100u" H 8488 4105 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 8400 4150 50  0001 C CNN
F 3 "~" H 8400 4150 50  0001 C CNN
	1    8400 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E07611A
P 8400 4250
F 0 "#PWR0102" H 8400 4000 50  0001 C CNN
F 1 "GND" H 8405 4077 50  0000 C CNN
F 2 "" H 8400 4250 50  0001 C CNN
F 3 "" H 8400 4250 50  0001 C CNN
	1    8400 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 4050 8400 3950
Wire Wire Line
	7850 3950 8400 3950
Wire Wire Line
	8400 3950 8700 3950
Connection ~ 8400 3950
Wire Wire Line
	7550 3850 7550 3950
Connection ~ 7550 3950
Wire Wire Line
	7550 3950 7650 3950
$Comp
L Connector:Conn_01x05_Male J3
U 1 1 5E08AE58
P 6050 5750
F 0 "J3" H 6022 5682 50  0000 R CNN
F 1 "Conn_01x05_Male" H 6022 5773 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S5B-XH-A-1_1x05_P2.50mm_Horizontal" H 6050 5750 50  0001 C CNN
F 3 "~" H 6050 5750 50  0001 C CNN
	1    6050 5750
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 5E08CBDF
P 5400 5850
F 0 "#PWR013" H 5400 5700 50  0001 C CNN
F 1 "+5V" V 5415 5978 50  0000 L CNN
F 2 "" H 5400 5850 50  0001 C CNN
F 3 "" H 5400 5850 50  0001 C CNN
	1    5400 5850
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5E08D696
P 5400 5950
F 0 "#PWR014" H 5400 5700 50  0001 C CNN
F 1 "GND" H 5405 5777 50  0000 C CNN
F 2 "" H 5400 5950 50  0001 C CNN
F 3 "" H 5400 5950 50  0001 C CNN
	1    5400 5950
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 5850 5850 5850
Wire Wire Line
	5850 5950 5400 5950
NoConn ~ 2100 4300
NoConn ~ 2100 4400
NoConn ~ 2100 5075
NoConn ~ 2100 4875
NoConn ~ 2100 4775
NoConn ~ 2100 4675
NoConn ~ 2100 4575
NoConn ~ 2100 5175
NoConn ~ 2100 5275
NoConn ~ 2100 5375
NoConn ~ 2100 5475
NoConn ~ 2100 5575
NoConn ~ 2100 5675
NoConn ~ 2100 5775
NoConn ~ 2100 5975
NoConn ~ 2100 6075
NoConn ~ 2100 6175
NoConn ~ 2100 6275
NoConn ~ 4425 6150
NoConn ~ 4425 6050
NoConn ~ 4425 5850
NoConn ~ 4425 5750
NoConn ~ 4425 5650
NoConn ~ 4425 5550
NoConn ~ 4425 5450
NoConn ~ 4425 5250
NoConn ~ 4425 5150
NoConn ~ 4425 5050
NoConn ~ 4425 4950
NoConn ~ 4425 4850
NoConn ~ 4425 4750
NoConn ~ 4425 4650
NoConn ~ 4425 4550
NoConn ~ 4425 4400
NoConn ~ 4425 4300
NoConn ~ 4425 4100
NoConn ~ 4425 4000
NoConn ~ 4425 3900
NoConn ~ 4425 3800
NoConn ~ 4425 3700
NoConn ~ 4425 3600
NoConn ~ 4425 3500
NoConn ~ 4425 3400
Wire Wire Line
	4425 1700 4450 1700
NoConn ~ 4425 1600
NoConn ~ 4425 1500
NoConn ~ 4425 1400
Wire Wire Line
	4425 2200 4700 2200
Wire Wire Line
	4425 2000 4700 2000
Wire Wire Line
	4425 2100 4700 2100
Connection ~ 2100 2200
Wire Wire Line
	4425 2300 4700 2300
Wire Wire Line
	4425 2500 4700 2500
$Comp
L schema-rescue:ARDUINO_MEGA-shield_arduino-test-rescue SH1
U 1 1 5E01C684
P 3250 3800
F 0 "SH1" H 3262 6593 60  0000 C CNN
F 1 "ARDUINO_MEGA" H 3262 6487 60  0000 C CNN
F 2 "SHIELD_ARDUINO:ARDUINO_MEGA_2560_R3_ELL" H 2300 4100 60  0001 C CNN
F 3 "http://arduino.cc/en/Main/ArduinoBoardMega2560" H 3262 6381 60  0000 C CNN
	1    3250 3800
	1    0    0    -1  
$EndComp
NoConn ~ 2100 1700
NoConn ~ 2100 1800
NoConn ~ 2100 1900
NoConn ~ 7200 1650
Wire Wire Line
	7000 2250 7200 2250
Wire Wire Line
	7000 2350 7200 2350
Wire Wire Line
	7200 1850 7000 1850
Wire Wire Line
	7200 1950 7000 1950
Entry Wire Line
	4750 5750 4650 5850
Entry Wire Line
	4750 5650 4650 5750
Entry Wire Line
	4750 5550 4650 5650
Wire Wire Line
	4750 5550 5850 5550
Wire Wire Line
	4750 5650 5850 5650
Wire Wire Line
	4750 5750 5850 5750
Entry Wire Line
	1750 2400 1850 2500
Entry Wire Line
	1750 2500 1850 2600
Text Label 1850 2500 0    50   ~ 0
A0
Text Label 1850 2600 0    50   ~ 0
A1
Text Label 4750 5650 0    50   ~ 0
A9
Text Label 4750 5750 0    50   ~ 0
A10
Entry Wire Line
	1850 2700 1750 2600
Text Label 1850 2700 0    50   ~ 0
A2
Text Label 4750 5550 0    50   ~ 0
A8
Wire Wire Line
	2100 2500 1850 2500
Wire Wire Line
	2100 2600 1850 2600
Wire Wire Line
	2100 2700 1850 2700
Wire Bus Line
	1750 6600 4650 6600
Entry Wire Line
	1750 3300 1850 3400
Wire Wire Line
	2100 3400 1850 3400
Text Label 1850 3400 0    50   ~ 0
A8
Entry Wire Line
	1750 2700 1850 2800
Entry Wire Line
	1750 2800 1850 2900
Entry Wire Line
	1750 2900 1850 3000
Entry Wire Line
	1750 3000 1850 3100
Entry Wire Line
	1750 3100 1850 3200
Wire Wire Line
	2100 2800 1850 2800
Wire Wire Line
	1850 2900 2100 2900
Wire Wire Line
	2100 3000 1850 3000
Wire Wire Line
	1850 3100 2100 3100
Wire Wire Line
	2100 3200 1850 3200
Text Label 1850 2800 0    50   ~ 0
A3
Text Label 1850 2900 0    50   ~ 0
A4
Text Label 1850 3000 0    50   ~ 0
A5
Text Label 1850 3100 0    50   ~ 0
A6
Text Label 1850 3200 0    50   ~ 0
A7
Entry Wire Line
	4700 2600 4800 2500
Entry Wire Line
	4700 2700 4800 2600
Entry Wire Line
	4700 2800 4800 2700
Entry Wire Line
	4700 2900 4800 2800
Entry Wire Line
	4700 3000 4800 2900
Entry Wire Line
	4700 3100 4800 3000
Entry Wire Line
	4700 3200 4800 3100
Wire Wire Line
	4700 3200 4425 3200
Wire Wire Line
	4425 3100 4700 3100
Wire Wire Line
	4425 2900 4700 2900
Wire Wire Line
	4425 3000 4700 3000
Wire Wire Line
	4700 2800 4425 2800
Wire Wire Line
	4425 2700 4700 2700
Wire Wire Line
	4425 2600 4700 2600
Text Label 4625 2600 0    50   ~ 0
6
Text Label 4625 2700 0    50   ~ 0
5
Text Label 4625 2800 0    50   ~ 0
4
Text Label 4625 2900 0    50   ~ 0
3
Text Label 4625 3000 0    50   ~ 0
2
Text Label 4625 3100 0    50   ~ 0
1
Text Label 4625 3200 0    50   ~ 0
0
Wire Wire Line
	7000 4300 7250 4300
Wire Wire Line
	7000 4600 7250 4600
Wire Wire Line
	7000 4700 7250 4700
Wire Wire Line
	7000 4800 7250 4800
Wire Wire Line
	7000 5000 7250 5000
Wire Wire Line
	7000 5100 7250 5100
Wire Wire Line
	7000 5200 7250 5200
Wire Bus Line
	4650 5350 4650 6600
Wire Bus Line
	6900 900  6900 5200
Wire Bus Line
	4800 900  4800 3850
Wire Bus Line
	1750 900  1750 6600
Connection ~ 4800 900 
Wire Bus Line
	4800 900  6900 900 
$EndSCHEMATC
