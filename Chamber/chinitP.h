//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chinitP.h,v 2.9 1996-12-12 15:31:33 distler Exp $
//
// INIT file fuer die chamber++ library.
// Offset und Position fuer die Draehte
// von allen beiden Ebenen der Prototyp
// Driftkammer.
//

#ifndef __chinitP__
#define __chinitP__

const short CH_P_MAX_X1		= 96;
const short CH_P_MAX_S1		= 80;
const short CH_P_MAX_X2		= 2;
const short CH_P_MAX_S2		= 2;

const short CH_P_OFFSET_X1	= 512;
const short CH_P_OFFSET_S1	= 512;
const short CH_P_OFFSET_X2	= 512;
const short CH_P_OFFSET_S2	= 512;

const double CH_P_ANGLE		= 45.0;
const short  CH_P_FULLRANGE	= 512;
const double CH_P_TIMETOLENGTH	= 0.5 * 0.052;

static short
offset_px1[CH_P_MAX_X1] = {
  CH_P_OFFSET_X1,		// wire 1
  CH_P_OFFSET_X1,		// wire 2
  CH_P_OFFSET_X1-2,		// wire 3
  CH_P_OFFSET_X1-0,		// wire 4
  CH_P_OFFSET_X1-0,		// wire 5
  CH_P_OFFSET_X1,		// wire 6
  CH_P_OFFSET_X1,		// wire 7
  CH_P_OFFSET_X1,		// wire 8
  CH_P_OFFSET_X1,		// wire 9
  CH_P_OFFSET_X1,		// wire 10
  CH_P_OFFSET_X1,		// wire 11
  CH_P_OFFSET_X1,		// wire 12
  CH_P_OFFSET_X1,		// wire 13
  CH_P_OFFSET_X1-0,		// wire 14
  CH_P_OFFSET_X1-0,		// wire 15
  CH_P_OFFSET_X1-0,		// wire 16
  CH_P_OFFSET_X1-0,		// wire 17
  CH_P_OFFSET_X1-0,		// wire 18
  CH_P_OFFSET_X1-4,		// wire 19
  CH_P_OFFSET_X1-2,		// wire 20
  CH_P_OFFSET_X1-3,		// wire 21
  CH_P_OFFSET_X1-3,		// wire 22
  CH_P_OFFSET_X1-4,		// wire 23
  CH_P_OFFSET_X1-0,		// wire 24
  CH_P_OFFSET_X1-0,		// wire 25
  CH_P_OFFSET_X1-0,		// wire 26
  CH_P_OFFSET_X1-2,		// wire 27
  CH_P_OFFSET_X1-1,		// wire 28
  CH_P_OFFSET_X1-1,		// wire 29
  CH_P_OFFSET_X1-2,		// wire 30
  CH_P_OFFSET_X1-5,		// wire 31
  CH_P_OFFSET_X1-2,		// wire 32
  CH_P_OFFSET_X1-4,		// wire 33
  CH_P_OFFSET_X1-2,		// wire 34
  CH_P_OFFSET_X1-4,		// wire 35
  CH_P_OFFSET_X1-4,		// wire 36
  CH_P_OFFSET_X1-8,		// wire 37
  CH_P_OFFSET_X1-7,		// wire 38
  CH_P_OFFSET_X1-11,		// wire 39
  CH_P_OFFSET_X1-3,		// wire 40
  CH_P_OFFSET_X1-5,		// wire 41
  CH_P_OFFSET_X1-9,		// wire 42
  CH_P_OFFSET_X1-11,		// wire 43
  CH_P_OFFSET_X1-10,		// wire 44
  CH_P_OFFSET_X1-16,		// wire 45
  CH_P_OFFSET_X1-8,		// wire 46
  CH_P_OFFSET_X1-9,		// wire 47
  CH_P_OFFSET_X1-12,		// wire 48
  CH_P_OFFSET_X1,		// wire 49
  CH_P_OFFSET_X1,		// wire 50
  CH_P_OFFSET_X1,		// wire 51
  CH_P_OFFSET_X1,		// wire 52
  CH_P_OFFSET_X1,		// wire 53
  CH_P_OFFSET_X1,		// wire 54
  CH_P_OFFSET_X1,		// wire 55
  CH_P_OFFSET_X1,		// wire 56
  CH_P_OFFSET_X1,		// wire 57
  CH_P_OFFSET_X1,		// wire 58
  CH_P_OFFSET_X1,		// wire 59
  CH_P_OFFSET_X1,		// wire 60
  CH_P_OFFSET_X1,		// wire 61
  CH_P_OFFSET_X1,		// wire 62
  CH_P_OFFSET_X1,		// wire 63
  CH_P_OFFSET_X1,		// wire 64
  CH_P_OFFSET_X1,		// wire 65
  CH_P_OFFSET_X1,		// wire 66
  CH_P_OFFSET_X1,		// wire 67
  CH_P_OFFSET_X1,		// wire 68
  CH_P_OFFSET_X1,		// wire 69
  CH_P_OFFSET_X1,		// wire 70
  CH_P_OFFSET_X1,		// wire 71
  CH_P_OFFSET_X1,		// wire 72
  CH_P_OFFSET_X1,		// wire 73
  CH_P_OFFSET_X1,		// wire 74
  CH_P_OFFSET_X1,		// wire 75
  CH_P_OFFSET_X1,		// wire 76
  CH_P_OFFSET_X1,		// wire 77
  CH_P_OFFSET_X1,		// wire 78
  CH_P_OFFSET_X1,		// wire 79
  CH_P_OFFSET_X1,		// wire 80
  CH_P_OFFSET_X1,		// wire 81
  CH_P_OFFSET_X1,		// wire 82
  CH_P_OFFSET_X1,		// wire 83
  CH_P_OFFSET_X1,		// wire 84
  CH_P_OFFSET_X1,		// wire 85
  CH_P_OFFSET_X1,		// wire 86
  CH_P_OFFSET_X1,		// wire 87
  CH_P_OFFSET_X1,		// wire 88
  CH_P_OFFSET_X1,		// wire 89
  CH_P_OFFSET_X1,		// wire 90
  CH_P_OFFSET_X1,		// wire 91
  CH_P_OFFSET_X1,		// wire 92
  CH_P_OFFSET_X1,		// wire 93
  CH_P_OFFSET_X1,		// wire 94
  CH_P_OFFSET_X1,		// wire 95
  CH_P_OFFSET_X1		// wire 96
};

static short
offset_ps1[CH_P_MAX_S1] = {
  CH_P_OFFSET_S1,		// wire 1
  CH_P_OFFSET_S1,		// wire 2
  CH_P_OFFSET_S1,		// wire 3
  CH_P_OFFSET_S1,		// wire 4
  CH_P_OFFSET_S1,		// wire 5
  CH_P_OFFSET_S1-14,		// wire 6
  CH_P_OFFSET_S1-11,		// wire 7
  CH_P_OFFSET_S1-8,		// wire 8
  CH_P_OFFSET_S1-7,		// wire 9
  CH_P_OFFSET_S1-6,		// wire 10
  CH_P_OFFSET_S1-8,		// wire 11
  CH_P_OFFSET_S1-6,		// wire 12
  CH_P_OFFSET_S1,		// wire 13
  CH_P_OFFSET_S1-5,		// wire 14
  CH_P_OFFSET_S1-9,		// wire 15
  CH_P_OFFSET_S1-2,		// wire 16
  CH_P_OFFSET_S1-3,		// wire 17
  CH_P_OFFSET_S1-3,		// wire 18
  CH_P_OFFSET_S1-4,		// wire 19
  CH_P_OFFSET_S1-3,		// wire 20
  CH_P_OFFSET_S1-5,		// wire 21
  CH_P_OFFSET_S1-5,		// wire 22
  CH_P_OFFSET_S1-7,		// wire 23
  CH_P_OFFSET_S1-0,		// wire 24
  CH_P_OFFSET_S1-2,		// wire 25
  CH_P_OFFSET_S1-2,		// wire 26
  CH_P_OFFSET_S1-3,		// wire 27
  CH_P_OFFSET_S1-3,		// wire 28
  CH_P_OFFSET_S1-2,		// wire 29
  CH_P_OFFSET_S1-4,		// wire 30
  CH_P_OFFSET_S1-7,		// wire 31
  CH_P_OFFSET_S1-3,		// wire 32
  CH_P_OFFSET_S1-5,		// wire 33
  CH_P_OFFSET_S1-2,		// wire 34
  CH_P_OFFSET_S1-6,		// wire 35
  CH_P_OFFSET_S1-6,		// wire 36
  CH_P_OFFSET_S1-5,		// wire 37
  CH_P_OFFSET_S1-7,		// wire 38
  CH_P_OFFSET_S1-6,		// wire 39
  CH_P_OFFSET_S1-9,		// wire 40
  CH_P_OFFSET_S1-11,		// wire 41
  CH_P_OFFSET_S1-8,		// wire 42
  CH_P_OFFSET_S1-10,		// wire 43
  CH_P_OFFSET_S1-11,		// wire 44
  CH_P_OFFSET_S1-15,		// wire 45
  CH_P_OFFSET_S1-14,		// wire 46
  CH_P_OFFSET_S1-10,		// wire 47
  CH_P_OFFSET_S1-11,		// wire 48
  CH_P_OFFSET_S1-10,		// wire 49
  CH_P_OFFSET_S1-12,		// wire 50
  CH_P_OFFSET_S1-11,		// wire 51
  CH_P_OFFSET_S1-13,		// wire 52
  CH_P_OFFSET_S1,		// wire 53
  CH_P_OFFSET_S1,		// wire 54
  CH_P_OFFSET_S1,		// wire 55
  CH_P_OFFSET_S1,		// wire 56
  CH_P_OFFSET_S1,		// wire 57
  CH_P_OFFSET_S1,		// wire 58
  CH_P_OFFSET_S1,		// wire 59
  CH_P_OFFSET_S1,		// wire 60
  CH_P_OFFSET_S1,		// wire 61
  CH_P_OFFSET_S1,		// wire 62
  CH_P_OFFSET_S1,		// wire 63
  CH_P_OFFSET_S1,		// wire 64
  CH_P_OFFSET_S1,		// wire 65
  CH_P_OFFSET_S1,		// wire 66
  CH_P_OFFSET_S1,		// wire 67
  CH_P_OFFSET_S1,		// wire 68
  CH_P_OFFSET_S1,		// wire 69
  CH_P_OFFSET_S1,		// wire 70
  CH_P_OFFSET_S1,		// wire 71
  CH_P_OFFSET_S1,		// wire 72
  CH_P_OFFSET_S1,		// wire 73
  CH_P_OFFSET_S1,		// wire 74
  CH_P_OFFSET_S1,		// wire 75
  CH_P_OFFSET_S1,		// wire 76
  CH_P_OFFSET_S1,		// wire 77
  CH_P_OFFSET_S1,		// wire 78
  CH_P_OFFSET_S1,		// wire 79
  CH_P_OFFSET_S1		// wire 80
};

static short
offset_px2[CH_P_MAX_X2] = {
  CH_P_OFFSET_X2,		// wire 1
  CH_P_OFFSET_X2		// wire 2
};

static short
offset_ps2[CH_P_MAX_S2] = {
  CH_P_OFFSET_S2,		// wire 1
  CH_P_OFFSET_S2		// wire 2
};

static float
position_px1[CH_P_MAX_X1] = {
  1420.57,		// x1 wire 1
  1415.57,		// x1 wire 2
  1410.57,		// x1 wire 3
  1405.57,		// x1 wire 4
  1400.57,		// x1 wire 5
  1395.57,		// x1 wire 6
  1390.57,		// x1 wire 7
  1385.57,		// x1 wire 8
  1380.57,		// x1 wire 9
  1375.57,		// x1 wire 10
  1370.57,		// x1 wire 11
  1365.57,		// x1 wire 12
  1360.57,		// x1 wire 13
  1355.57,		// x1 wire 14
  1350.57,		// x1 wire 15
  1345.57,		// x1 wire 16
  1340.57,		// x1 wire 17
  1335.57,		// x1 wire 18
  1330.57,		// x1 wire 19
  1325.57,		// x1 wire 20
  1320.57,		// x1 wire 21
  1315.57,		// x1 wire 22
  1310.57,		// x1 wire 23
  1305.57,		// x1 wire 24
  1300.57,		// x1 wire 25
  1295.57,		// x1 wire 26
  1290.57,		// x1 wire 27
  1285.57,		// x1 wire 28
  1280.57,		// x1 wire 29
  1275.57,		// x1 wire 30
  1270.57,		// x1 wire 31
  1265.57,		// x1 wire 32
  1260.57,		// x1 wire 33
  1255.57,		// x1 wire 34
  1250.57,		// x1 wire 35
  1245.57,		// x1 wire 36
  1240.57,		// x1 wire 37
  1235.57,		// x1 wire 38
  1230.57,		// x1 wire 39
  1225.57,		// x1 wire 40
  1220.57,		// x1 wire 41
  1215.57,		// x1 wire 42
  1210.57,		// x1 wire 43
  1205.57,		// x1 wire 44
  1200.57,		// x1 wire 45
  1195.57,		// x1 wire 46
  1190.57,		// x1 wire 47
  1185.57,		// x1 wire 48
  1180.57,		// x1 wire 49
  1175.57,		// x1 wire 50
  1170.57,		// x1 wire 51
  1165.57,		// x1 wire 52
  1160.57,		// x1 wire 53
  1155.57,		// x1 wire 54
  1150.57,		// x1 wire 55
  1145.57,		// x1 wire 56
  1140.57,		// x1 wire 57
  1135.57,		// x1 wire 58
  1130.57,		// x1 wire 59
  1125.57,		// x1 wire 60
  1120.57,		// x1 wire 61
  1115.57,		// x1 wire 62
  1110.57,		// x1 wire 63
  1105.57,		// x1 wire 64
  1100.57,		// x1 wire 65
  1095.57,		// x1 wire 66
  1090.57,		// x1 wire 67
  1085.57,		// x1 wire 68
  1080.57,		// x1 wire 69
  1075.57,		// x1 wire 70
  1070.57,		// x1 wire 71
  1065.57,		// x1 wire 72
  1060.57,		// x1 wire 73
  1055.57,		// x1 wire 74
  1050.57,		// x1 wire 75
  1045.57,		// x1 wire 76
  1040.57,		// x1 wire 77
  1035.57,		// x1 wire 78
  1030.57,		// x1 wire 79
  1025.57,		// x1 wire 80
  1020.57,		// x1 wire 81
  1015.57,		// x1 wire 82
  1010.57,		// x1 wire 83
  1005.57,		// x1 wire 84
  1000.57,		// x1 wire 85
   995.57,		// x1 wire 86
   990.57,		// x1 wire 87
   985.57,		// x1 wire 88
   980.57,		// x1 wire 89
   975.57,		// x1 wire 90
   970.57,		// x1 wire 91
   965.57,		// x1 wire 92
   960.57,		// x1 wire 93
   955.57,		// x1 wire 94
   950.57,		// x1 wire 95
   945.57		// x1 wire 96
};

static float
position_ps1[CH_P_MAX_S1] = {
  1161.04,		// s1 wire 1
  1156.04,		// s1 wire 2
  1151.04,		// s1 wire 3
  1146.04,		// s1 wire 4
  1141.04,		// s1 wire 5
  1136.04,		// s1 wire 6
  1131.04,		// s1 wire 7
  1126.04,		// s1 wire 8
  1121.04,		// s1 wire 9
  1116.04,		// s1 wire 10
  1111.04,		// s1 wire 11
  1106.04,		// s1 wire 12
  1101.04,		// s1 wire 13
  1096.04,		// s1 wire 14
  1091.04,		// s1 wire 15
  1086.04,		// s1 wire 16
  1081.04,		// s1 wire 17
  1076.04,		// s1 wire 18
  1071.04,		// s1 wire 19
  1066.04,		// s1 wire 20
  1061.04,		// s1 wire 21
  1056.04,		// s1 wire 22
  1051.04,		// s1 wire 23
  1046.04,		// s1 wire 24
  1041.04,		// s1 wire 25
  1036.04,		// s1 wire 26
  1031.04,		// s1 wire 27
  1026.04,		// s1 wire 28
  1021.04,		// s1 wire 29
  1016.04,		// s1 wire 30
  1011.04,		// s1 wire 31
  1006.04,		// s1 wire 32
  1001.04,		// s1 wire 33
   996.04,		// s1 wire 34
   991.04,		// s1 wire 35
   986.04,		// s1 wire 36
   981.04,		// s1 wire 37
   976.04,		// s1 wire 38
   971.04,		// s1 wire 39
   966.04,		// s1 wire 40
   961.04,		// s1 wire 41
   956.04,		// s1 wire 42
   951.04,		// s1 wire 43
   946.04,		// s1 wire 44
   941.04,		// s1 wire 45
   936.04,		// s1 wire 46
   931.04,		// s1 wire 47
   926.04,		// s1 wire 48
   921.04,		// s1 wire 49
   916.04,		// s1 wire 50
   911.04,		// s1 wire 51
   906.04,		// s1 wire 52
   901.04,		// s1 wire 53
   896.04,		// s1 wire 54
   891.04,		// s1 wire 55
   886.04,		// s1 wire 56
   881.04,		// s1 wire 57
   876.04,		// s1 wire 58
   871.04,		// s1 wire 59
   866.04,		// s1 wire 60
   861.04,		// s1 wire 61
   856.04,		// s1 wire 62
   851.04,		// s1 wire 63
   846.04,		// s1 wire 64
   841.04,		// s1 wire 65
   836.04,		// s1 wire 66
   831.04,		// s1 wire 67
   826.04,		// s1 wire 68
   821.04,		// s1 wire 69
   816.04,		// s1 wire 70
   811.04,		// s1 wire 71
   806.04,		// s1 wire 72
   801.04,		// s1 wire 73
   796.04,		// s1 wire 74
   791.04,		// s1 wire 75
   786.04,		// s1 wire 76
   781.04,		// s1 wire 77
   776.04,		// s1 wire 78
   771.04,		// s1 wire 79
   766.04		// s1 wire 80
};

static float
position_px2[CH_P_MAX_X2] = {
  1772.57,		// x2 wire 1
  1767.57		// x2 wire 2
};

static float
position_ps2[CH_P_MAX_S2] = {
  1430.69,		// s2 wire 1
  1425.69		// s2 wire 2
};

vdcPosOffs vdcPosOffsProto = {
  { position_px1, offset_px1, CH_P_MAX_X1 },
  { position_ps1, offset_ps1, CH_P_MAX_S1 },
  { position_px2, offset_px2, CH_P_MAX_X2 },
  { position_ps2, offset_ps2, CH_P_MAX_S2 }
};

#endif
