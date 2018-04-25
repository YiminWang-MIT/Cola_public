//								      -*-c++-*-
// Created 1997 by	Michael O. Distler,
//			MIT, Cambridge, MA
//
// $Header: /tmp/cvsroot/Cola/Chamber/chinitO.h,v 1.9 2002-05-28 21:20:40 cola Exp $
//
// Initialize wire position and offset of OHIPS chambers.
//

#ifndef __chinitO__
#define __chinitO__

#include <math.h>

const short CH_OHIPS_MAX_U1	= 256;
const short CH_OHIPS_MAX_V1	= 256;
const short CH_OHIPS_MAX_U2	= 256;
const short CH_OHIPS_MAX_V2	= 256;

const short CH_OHIPS_OFFSET_U1	= 450;
const short CH_OHIPS_OFFSET_V1	= 447;
const short CH_OHIPS_OFFSET_U2	= 451;
const short CH_OHIPS_OFFSET_V2	= 452;

const double CH_OHIPS_CORR1[5]	= { 2.34, -.2507, .7185, .5042, -.05379 };
const double CH_OHIPS_CORR2[5]	= { 2.20, -.2387, .6838, .4701, -.04998 };

const double CH_OHIPS_ANGLE	   = 45.0;
const short  CH_OHIPS_FULLRANGE	   = 512;
const double CH_OHIPS_TIMETOLENGTH = 1.0 * 0.050;
const double CH_OHIPS_DIST_U1_U2   = 146.0;
const double CH_OHIPS_DIST_V1_V2   = 131.5;
const double CH_OHIPS_DIST_U1_V1   = 38.1;

const double CH_OHIPS_POSITION_U1  =
- 176.0/2.*25.4/sqrt(32.0) - 4.395 - 1.139;
const double CH_OHIPS_POSITION_V1  =
- 176.0/2.*25.4/sqrt(32.0) - 6.804 + 8.384;
const double CH_OHIPS_POSITION_U2  =
- 100.0*sqrt(18.0) + 146.0/sqrt(2.0) - 28.808 + 1.202;
const double CH_OHIPS_POSITION_V2  =
- 100.0*sqrt(18.0) + 146.0/sqrt(2.0) - 27.671 + 3.327;

const double CH_OHIPS_TIMETOLENGTH_98 = .0458;
const double CH_OHIPS_DIST_U1_U2_98   = 163.;
const double CH_OHIPS_DIST_V1_V2_98   = 150.;
const double CH_OHIPS_DIST_U1_V1_98   = 36.52;
const double CH_OHIPS_CORR1_98[5] = { .91, -.093742, .29683, .19401, -.02086 };

const double CH_OHIPS_POSITION_U1_98 =
-48.0*sqrt(2.0) - 127.0/2.0*25.4/6.0 - 4.395;
const double CH_OHIPS_POSITION_V1_98 =
-48.0*sqrt(2.0) - 127.0/2.0*25.4/6.0 - 6.804;
const double CH_OHIPS_POSITION_U2_98 =
-48.0*sqrt(2.0) - 100.0*sqrt(18.0) + 162.9858/sqrt(2.0) - 28.808;
const double CH_OHIPS_POSITION_V2_98 =
-48.0*sqrt(2.0) - 100.0*sqrt(18.0) + 162.9858/sqrt(2.0) - 27.671;

static short
offset_ou1[CH_OHIPS_MAX_U1] = {
  CH_OHIPS_OFFSET_U1,			// wire 1
  CH_OHIPS_OFFSET_U1,			// wire 2
  CH_OHIPS_OFFSET_U1,			// wire 3
  CH_OHIPS_OFFSET_U1,			// wire 4
  CH_OHIPS_OFFSET_U1,			// wire 5
  CH_OHIPS_OFFSET_U1,			// wire 6
  CH_OHIPS_OFFSET_U1,			// wire 7
  CH_OHIPS_OFFSET_U1,			// wire 8
  CH_OHIPS_OFFSET_U1,			// wire 9
  CH_OHIPS_OFFSET_U1,			// wire 10
  CH_OHIPS_OFFSET_U1,			// wire 11
  CH_OHIPS_OFFSET_U1,			// wire 12
  CH_OHIPS_OFFSET_U1,			// wire 13
  CH_OHIPS_OFFSET_U1,			// wire 14
  CH_OHIPS_OFFSET_U1,			// wire 15
  CH_OHIPS_OFFSET_U1,			// wire 16
  CH_OHIPS_OFFSET_U1,			// wire 17
  CH_OHIPS_OFFSET_U1,			// wire 18
  CH_OHIPS_OFFSET_U1,			// wire 19
  CH_OHIPS_OFFSET_U1,			// wire 20
  CH_OHIPS_OFFSET_U1,			// wire 21
  CH_OHIPS_OFFSET_U1,			// wire 22
  CH_OHIPS_OFFSET_U1,			// wire 23
  CH_OHIPS_OFFSET_U1,			// wire 24
  CH_OHIPS_OFFSET_U1,			// wire 25
  CH_OHIPS_OFFSET_U1,			// wire 26
  CH_OHIPS_OFFSET_U1,			// wire 27
  CH_OHIPS_OFFSET_U1,			// wire 28
  CH_OHIPS_OFFSET_U1,			// wire 29
  CH_OHIPS_OFFSET_U1,			// wire 30
  CH_OHIPS_OFFSET_U1,			// wire 31
  CH_OHIPS_OFFSET_U1,			// wire 32
  CH_OHIPS_OFFSET_U1,			// wire 33
  CH_OHIPS_OFFSET_U1,			// wire 34
  CH_OHIPS_OFFSET_U1,			// wire 35
  CH_OHIPS_OFFSET_U1,			// wire 36
  CH_OHIPS_OFFSET_U1,			// wire 37
  CH_OHIPS_OFFSET_U1,			// wire 38
  CH_OHIPS_OFFSET_U1,			// wire 39
  CH_OHIPS_OFFSET_U1,			// wire 40
  CH_OHIPS_OFFSET_U1,			// wire 41
  CH_OHIPS_OFFSET_U1,			// wire 42
  CH_OHIPS_OFFSET_U1,			// wire 43
  CH_OHIPS_OFFSET_U1,			// wire 44
  CH_OHIPS_OFFSET_U1,			// wire 45
  CH_OHIPS_OFFSET_U1,			// wire 46
  CH_OHIPS_OFFSET_U1,			// wire 47
  CH_OHIPS_OFFSET_U1,			// wire 48
  CH_OHIPS_OFFSET_U1,			// wire 49
  CH_OHIPS_OFFSET_U1,			// wire 50
  CH_OHIPS_OFFSET_U1,			// wire 51
  CH_OHIPS_OFFSET_U1,			// wire 52
  CH_OHIPS_OFFSET_U1,			// wire 53
  CH_OHIPS_OFFSET_U1,			// wire 54
  CH_OHIPS_OFFSET_U1,			// wire 55
  CH_OHIPS_OFFSET_U1,			// wire 56
  CH_OHIPS_OFFSET_U1,			// wire 57
  CH_OHIPS_OFFSET_U1,			// wire 58
  CH_OHIPS_OFFSET_U1,			// wire 59
  CH_OHIPS_OFFSET_U1,			// wire 60
  CH_OHIPS_OFFSET_U1,			// wire 61
  CH_OHIPS_OFFSET_U1,			// wire 62
  CH_OHIPS_OFFSET_U1,			// wire 63
  CH_OHIPS_OFFSET_U1,			// wire 64
  CH_OHIPS_OFFSET_U1,			// wire 65
  CH_OHIPS_OFFSET_U1,			// wire 66
  CH_OHIPS_OFFSET_U1,			// wire 67
  CH_OHIPS_OFFSET_U1,			// wire 68
  CH_OHIPS_OFFSET_U1,			// wire 69
  CH_OHIPS_OFFSET_U1,			// wire 70
  CH_OHIPS_OFFSET_U1,			// wire 71
  CH_OHIPS_OFFSET_U1,			// wire 72
  CH_OHIPS_OFFSET_U1,			// wire 73
  CH_OHIPS_OFFSET_U1,			// wire 74
  CH_OHIPS_OFFSET_U1,			// wire 75
  CH_OHIPS_OFFSET_U1,			// wire 76
  CH_OHIPS_OFFSET_U1,			// wire 77
  CH_OHIPS_OFFSET_U1,			// wire 78
  CH_OHIPS_OFFSET_U1,			// wire 79
  CH_OHIPS_OFFSET_U1,			// wire 80
  CH_OHIPS_OFFSET_U1,			// wire 81
  CH_OHIPS_OFFSET_U1,			// wire 82
  CH_OHIPS_OFFSET_U1,			// wire 83
  CH_OHIPS_OFFSET_U1,			// wire 84
  CH_OHIPS_OFFSET_U1,			// wire 85
  CH_OHIPS_OFFSET_U1,			// wire 86
  CH_OHIPS_OFFSET_U1,			// wire 87
  CH_OHIPS_OFFSET_U1,			// wire 88
  CH_OHIPS_OFFSET_U1,			// wire 89
  CH_OHIPS_OFFSET_U1,			// wire 90
  CH_OHIPS_OFFSET_U1,			// wire 91
  CH_OHIPS_OFFSET_U1,			// wire 92
  CH_OHIPS_OFFSET_U1,			// wire 93
  CH_OHIPS_OFFSET_U1,			// wire 94
  CH_OHIPS_OFFSET_U1,			// wire 95
  CH_OHIPS_OFFSET_U1,			// wire 96
  CH_OHIPS_OFFSET_U1,			// wire 97
  CH_OHIPS_OFFSET_U1,			// wire 98
  CH_OHIPS_OFFSET_U1,			// wire 99
  CH_OHIPS_OFFSET_U1,			// wire 100
  CH_OHIPS_OFFSET_U1,			// wire 101
  CH_OHIPS_OFFSET_U1,			// wire 102
  CH_OHIPS_OFFSET_U1,			// wire 103
  CH_OHIPS_OFFSET_U1,			// wire 104
  CH_OHIPS_OFFSET_U1,			// wire 105
  CH_OHIPS_OFFSET_U1,			// wire 106
  CH_OHIPS_OFFSET_U1,			// wire 107
  CH_OHIPS_OFFSET_U1,			// wire 108
  CH_OHIPS_OFFSET_U1,			// wire 109
  CH_OHIPS_OFFSET_U1,			// wire 110
  CH_OHIPS_OFFSET_U1,			// wire 111
  CH_OHIPS_OFFSET_U1,			// wire 112
  CH_OHIPS_OFFSET_U1,			// wire 113
  CH_OHIPS_OFFSET_U1,			// wire 114
  CH_OHIPS_OFFSET_U1,			// wire 115
  CH_OHIPS_OFFSET_U1,			// wire 116
  CH_OHIPS_OFFSET_U1,			// wire 117
  CH_OHIPS_OFFSET_U1,			// wire 118
  CH_OHIPS_OFFSET_U1,			// wire 119
  CH_OHIPS_OFFSET_U1,			// wire 120
  CH_OHIPS_OFFSET_U1,			// wire 121
  CH_OHIPS_OFFSET_U1,			// wire 122
  CH_OHIPS_OFFSET_U1,			// wire 123
  CH_OHIPS_OFFSET_U1,			// wire 124
  CH_OHIPS_OFFSET_U1,			// wire 125
  CH_OHIPS_OFFSET_U1,			// wire 126
  CH_OHIPS_OFFSET_U1,			// wire 127
  CH_OHIPS_OFFSET_U1,			// wire 128
  CH_OHIPS_OFFSET_U1,			// wire 129
  CH_OHIPS_OFFSET_U1,			// wire 130
  CH_OHIPS_OFFSET_U1,			// wire 131
  CH_OHIPS_OFFSET_U1,			// wire 132
  CH_OHIPS_OFFSET_U1,			// wire 133
  CH_OHIPS_OFFSET_U1,			// wire 134
  CH_OHIPS_OFFSET_U1,			// wire 135
  CH_OHIPS_OFFSET_U1,			// wire 136
  CH_OHIPS_OFFSET_U1,			// wire 137
  CH_OHIPS_OFFSET_U1,			// wire 138
  CH_OHIPS_OFFSET_U1,			// wire 139
  CH_OHIPS_OFFSET_U1,			// wire 140
  CH_OHIPS_OFFSET_U1,			// wire 141
  CH_OHIPS_OFFSET_U1,			// wire 142
  CH_OHIPS_OFFSET_U1,			// wire 143
  CH_OHIPS_OFFSET_U1,			// wire 144
  CH_OHIPS_OFFSET_U1,			// wire 145
  CH_OHIPS_OFFSET_U1,			// wire 146
  CH_OHIPS_OFFSET_U1,			// wire 147
  CH_OHIPS_OFFSET_U1,			// wire 148
  CH_OHIPS_OFFSET_U1,			// wire 149
  CH_OHIPS_OFFSET_U1,			// wire 150
  CH_OHIPS_OFFSET_U1,			// wire 151
  CH_OHIPS_OFFSET_U1,			// wire 152
  CH_OHIPS_OFFSET_U1,			// wire 153
  CH_OHIPS_OFFSET_U1,			// wire 154
  CH_OHIPS_OFFSET_U1,			// wire 155
  CH_OHIPS_OFFSET_U1,			// wire 156
  CH_OHIPS_OFFSET_U1,			// wire 157
  CH_OHIPS_OFFSET_U1,			// wire 158
  CH_OHIPS_OFFSET_U1,			// wire 159
  CH_OHIPS_OFFSET_U1,			// wire 160
  CH_OHIPS_OFFSET_U1,			// wire 161
  CH_OHIPS_OFFSET_U1,			// wire 162
  CH_OHIPS_OFFSET_U1,			// wire 163
  CH_OHIPS_OFFSET_U1,			// wire 164
  CH_OHIPS_OFFSET_U1,			// wire 165
  CH_OHIPS_OFFSET_U1,			// wire 166
  CH_OHIPS_OFFSET_U1,			// wire 167
  CH_OHIPS_OFFSET_U1,			// wire 168
  CH_OHIPS_OFFSET_U1,			// wire 169
  CH_OHIPS_OFFSET_U1,			// wire 170
  CH_OHIPS_OFFSET_U1,			// wire 171
  CH_OHIPS_OFFSET_U1,			// wire 172
  CH_OHIPS_OFFSET_U1,			// wire 173
  CH_OHIPS_OFFSET_U1,			// wire 174
  CH_OHIPS_OFFSET_U1,			// wire 175
  CH_OHIPS_OFFSET_U1,			// wire 176
  CH_OHIPS_OFFSET_U1,			// wire 177
  CH_OHIPS_OFFSET_U1,			// wire 178
  CH_OHIPS_OFFSET_U1,			// wire 179
  CH_OHIPS_OFFSET_U1,			// wire 180
  CH_OHIPS_OFFSET_U1,			// wire 181
  CH_OHIPS_OFFSET_U1,			// wire 182
  CH_OHIPS_OFFSET_U1,			// wire 183
  CH_OHIPS_OFFSET_U1,			// wire 184
  CH_OHIPS_OFFSET_U1,			// wire 185
  CH_OHIPS_OFFSET_U1,			// wire 186
  CH_OHIPS_OFFSET_U1,			// wire 187
  CH_OHIPS_OFFSET_U1,			// wire 188
  CH_OHIPS_OFFSET_U1,			// wire 189
  CH_OHIPS_OFFSET_U1,			// wire 190
  CH_OHIPS_OFFSET_U1,			// wire 191
  CH_OHIPS_OFFSET_U1,			// wire 192
  CH_OHIPS_OFFSET_U1,			// wire 193
  CH_OHIPS_OFFSET_U1,			// wire 194
  CH_OHIPS_OFFSET_U1,			// wire 195
  CH_OHIPS_OFFSET_U1,			// wire 196
  CH_OHIPS_OFFSET_U1,			// wire 197
  CH_OHIPS_OFFSET_U1,			// wire 198
  CH_OHIPS_OFFSET_U1,			// wire 199
  CH_OHIPS_OFFSET_U1,			// wire 200
  CH_OHIPS_OFFSET_U1,			// wire 201
  CH_OHIPS_OFFSET_U1,			// wire 202
  CH_OHIPS_OFFSET_U1,			// wire 203
  CH_OHIPS_OFFSET_U1,			// wire 204
  CH_OHIPS_OFFSET_U1,			// wire 205
  CH_OHIPS_OFFSET_U1,			// wire 206
  CH_OHIPS_OFFSET_U1,			// wire 207
  CH_OHIPS_OFFSET_U1,			// wire 208
  CH_OHIPS_OFFSET_U1,			// wire 209
  CH_OHIPS_OFFSET_U1,			// wire 210
  CH_OHIPS_OFFSET_U1,			// wire 211
  CH_OHIPS_OFFSET_U1,			// wire 212
  CH_OHIPS_OFFSET_U1,			// wire 213
  CH_OHIPS_OFFSET_U1,			// wire 214
  CH_OHIPS_OFFSET_U1,			// wire 215
  CH_OHIPS_OFFSET_U1,			// wire 216
  CH_OHIPS_OFFSET_U1,			// wire 217
  CH_OHIPS_OFFSET_U1,			// wire 218
  CH_OHIPS_OFFSET_U1,			// wire 219
  CH_OHIPS_OFFSET_U1,			// wire 220
  CH_OHIPS_OFFSET_U1,			// wire 221
  CH_OHIPS_OFFSET_U1,			// wire 222
  CH_OHIPS_OFFSET_U1,			// wire 223
  CH_OHIPS_OFFSET_U1,			// wire 224
  CH_OHIPS_OFFSET_U1,			// wire 225
  CH_OHIPS_OFFSET_U1,			// wire 226
  CH_OHIPS_OFFSET_U1,			// wire 227
  CH_OHIPS_OFFSET_U1,			// wire 228
  CH_OHIPS_OFFSET_U1,			// wire 229
  CH_OHIPS_OFFSET_U1,			// wire 230
  CH_OHIPS_OFFSET_U1,			// wire 231
  CH_OHIPS_OFFSET_U1,			// wire 232
  CH_OHIPS_OFFSET_U1,			// wire 233
  CH_OHIPS_OFFSET_U1,			// wire 234
  CH_OHIPS_OFFSET_U1,			// wire 235
  CH_OHIPS_OFFSET_U1,			// wire 236
  CH_OHIPS_OFFSET_U1,			// wire 237
  CH_OHIPS_OFFSET_U1,			// wire 238
  CH_OHIPS_OFFSET_U1,			// wire 239
  CH_OHIPS_OFFSET_U1,			// wire 240
  CH_OHIPS_OFFSET_U1,			// wire 241
  CH_OHIPS_OFFSET_U1,			// wire 242
  CH_OHIPS_OFFSET_U1,			// wire 243
  CH_OHIPS_OFFSET_U1,			// wire 244
  CH_OHIPS_OFFSET_U1,			// wire 245
  CH_OHIPS_OFFSET_U1,			// wire 246
  CH_OHIPS_OFFSET_U1,			// wire 247
  CH_OHIPS_OFFSET_U1,			// wire 248
  CH_OHIPS_OFFSET_U1,			// wire 249
  CH_OHIPS_OFFSET_U1,			// wire 250
  CH_OHIPS_OFFSET_U1,			// wire 251
  CH_OHIPS_OFFSET_U1,			// wire 252
  CH_OHIPS_OFFSET_U1,			// wire 253
  CH_OHIPS_OFFSET_U1,			// wire 254
  CH_OHIPS_OFFSET_U1,			// wire 255
  CH_OHIPS_OFFSET_U1			// wire 256
};

static short
offset_ov1[CH_OHIPS_MAX_V1] = {
  CH_OHIPS_OFFSET_V1,			// wire 1
  CH_OHIPS_OFFSET_V1,			// wire 2
  CH_OHIPS_OFFSET_V1,			// wire 3
  CH_OHIPS_OFFSET_V1,			// wire 4
  CH_OHIPS_OFFSET_V1,			// wire 5
  CH_OHIPS_OFFSET_V1,			// wire 6
  CH_OHIPS_OFFSET_V1,			// wire 7
  CH_OHIPS_OFFSET_V1,			// wire 8
  CH_OHIPS_OFFSET_V1,			// wire 9
  CH_OHIPS_OFFSET_V1,			// wire 10
  CH_OHIPS_OFFSET_V1,			// wire 11
  CH_OHIPS_OFFSET_V1,			// wire 12
  CH_OHIPS_OFFSET_V1,			// wire 13
  CH_OHIPS_OFFSET_V1,			// wire 14
  CH_OHIPS_OFFSET_V1,			// wire 15
  CH_OHIPS_OFFSET_V1,			// wire 16
  CH_OHIPS_OFFSET_V1,			// wire 17
  CH_OHIPS_OFFSET_V1,			// wire 18
  CH_OHIPS_OFFSET_V1,			// wire 19
  CH_OHIPS_OFFSET_V1,			// wire 20
  CH_OHIPS_OFFSET_V1,			// wire 21
  CH_OHIPS_OFFSET_V1,			// wire 22
  CH_OHIPS_OFFSET_V1,			// wire 23
  CH_OHIPS_OFFSET_V1,			// wire 24
  CH_OHIPS_OFFSET_V1,			// wire 25
  CH_OHIPS_OFFSET_V1,			// wire 26
  CH_OHIPS_OFFSET_V1,			// wire 27
  CH_OHIPS_OFFSET_V1,			// wire 28
  CH_OHIPS_OFFSET_V1,			// wire 29
  CH_OHIPS_OFFSET_V1,			// wire 30
  CH_OHIPS_OFFSET_V1,			// wire 31
  CH_OHIPS_OFFSET_V1,			// wire 32
  CH_OHIPS_OFFSET_V1,			// wire 33
  CH_OHIPS_OFFSET_V1,			// wire 34
  CH_OHIPS_OFFSET_V1,			// wire 35
  CH_OHIPS_OFFSET_V1,			// wire 36
  CH_OHIPS_OFFSET_V1,			// wire 37
  CH_OHIPS_OFFSET_V1,			// wire 38
  CH_OHIPS_OFFSET_V1,			// wire 39
  CH_OHIPS_OFFSET_V1,			// wire 40
  CH_OHIPS_OFFSET_V1,			// wire 41
  CH_OHIPS_OFFSET_V1,			// wire 42
  CH_OHIPS_OFFSET_V1,			// wire 43
  CH_OHIPS_OFFSET_V1,			// wire 44
  CH_OHIPS_OFFSET_V1,			// wire 45
  CH_OHIPS_OFFSET_V1,			// wire 46
  CH_OHIPS_OFFSET_V1,			// wire 47
  CH_OHIPS_OFFSET_V1,			// wire 48
  CH_OHIPS_OFFSET_V1,			// wire 49
  CH_OHIPS_OFFSET_V1,			// wire 50
  CH_OHIPS_OFFSET_V1,			// wire 51
  CH_OHIPS_OFFSET_V1,			// wire 52
  CH_OHIPS_OFFSET_V1,			// wire 53
  CH_OHIPS_OFFSET_V1,			// wire 54
  CH_OHIPS_OFFSET_V1,			// wire 55
  CH_OHIPS_OFFSET_V1,			// wire 56
  CH_OHIPS_OFFSET_V1,			// wire 57
  CH_OHIPS_OFFSET_V1,			// wire 58
  CH_OHIPS_OFFSET_V1,			// wire 59
  CH_OHIPS_OFFSET_V1,			// wire 60
  CH_OHIPS_OFFSET_V1,			// wire 61
  CH_OHIPS_OFFSET_V1,			// wire 62
  CH_OHIPS_OFFSET_V1,			// wire 63
  CH_OHIPS_OFFSET_V1,			// wire 64
  CH_OHIPS_OFFSET_V1,			// wire 65
  CH_OHIPS_OFFSET_V1,			// wire 66
  CH_OHIPS_OFFSET_V1,			// wire 67
  CH_OHIPS_OFFSET_V1,			// wire 68
  CH_OHIPS_OFFSET_V1,			// wire 69
  CH_OHIPS_OFFSET_V1,			// wire 70
  CH_OHIPS_OFFSET_V1,			// wire 71
  CH_OHIPS_OFFSET_V1,			// wire 72
  CH_OHIPS_OFFSET_V1,			// wire 73
  CH_OHIPS_OFFSET_V1,			// wire 74
  CH_OHIPS_OFFSET_V1,			// wire 75
  CH_OHIPS_OFFSET_V1,			// wire 76
  CH_OHIPS_OFFSET_V1,			// wire 77
  CH_OHIPS_OFFSET_V1,			// wire 78
  CH_OHIPS_OFFSET_V1,			// wire 79
  CH_OHIPS_OFFSET_V1,			// wire 80
  CH_OHIPS_OFFSET_V1,			// wire 81
  CH_OHIPS_OFFSET_V1,			// wire 82
  CH_OHIPS_OFFSET_V1,			// wire 83
  CH_OHIPS_OFFSET_V1,			// wire 84
  CH_OHIPS_OFFSET_V1,			// wire 85
  CH_OHIPS_OFFSET_V1,			// wire 86
  CH_OHIPS_OFFSET_V1,			// wire 87
  CH_OHIPS_OFFSET_V1,			// wire 88
  CH_OHIPS_OFFSET_V1,			// wire 89
  CH_OHIPS_OFFSET_V1,			// wire 90
  CH_OHIPS_OFFSET_V1,			// wire 91
  CH_OHIPS_OFFSET_V1,			// wire 92
  CH_OHIPS_OFFSET_V1,			// wire 93
  CH_OHIPS_OFFSET_V1,			// wire 94
  CH_OHIPS_OFFSET_V1,			// wire 95
  CH_OHIPS_OFFSET_V1,			// wire 96
  CH_OHIPS_OFFSET_V1,			// wire 97
  CH_OHIPS_OFFSET_V1,			// wire 98
  CH_OHIPS_OFFSET_V1,			// wire 99
  CH_OHIPS_OFFSET_V1,			// wire 100
  CH_OHIPS_OFFSET_V1,			// wire 101
  CH_OHIPS_OFFSET_V1,			// wire 102
  CH_OHIPS_OFFSET_V1,			// wire 103
  CH_OHIPS_OFFSET_V1,			// wire 104
  CH_OHIPS_OFFSET_V1,			// wire 105
  CH_OHIPS_OFFSET_V1,			// wire 106
  CH_OHIPS_OFFSET_V1,			// wire 107
  CH_OHIPS_OFFSET_V1,			// wire 108
  CH_OHIPS_OFFSET_V1,			// wire 109
  CH_OHIPS_OFFSET_V1,			// wire 110
  CH_OHIPS_OFFSET_V1,			// wire 111
  CH_OHIPS_OFFSET_V1,			// wire 112
  CH_OHIPS_OFFSET_V1,			// wire 113
  CH_OHIPS_OFFSET_V1,			// wire 114
  CH_OHIPS_OFFSET_V1,			// wire 115
  CH_OHIPS_OFFSET_V1,			// wire 116
  CH_OHIPS_OFFSET_V1,			// wire 117
  CH_OHIPS_OFFSET_V1,			// wire 118
  CH_OHIPS_OFFSET_V1,			// wire 119
  CH_OHIPS_OFFSET_V1,			// wire 120
  CH_OHIPS_OFFSET_V1,			// wire 121
  CH_OHIPS_OFFSET_V1,			// wire 122
  CH_OHIPS_OFFSET_V1,			// wire 123
  CH_OHIPS_OFFSET_V1,			// wire 124
  CH_OHIPS_OFFSET_V1,			// wire 125
  CH_OHIPS_OFFSET_V1,			// wire 126
  CH_OHIPS_OFFSET_V1,			// wire 127
  CH_OHIPS_OFFSET_V1,			// wire 128
  CH_OHIPS_OFFSET_V1,			// wire 129
  CH_OHIPS_OFFSET_V1,			// wire 130
  CH_OHIPS_OFFSET_V1,			// wire 131
  CH_OHIPS_OFFSET_V1,			// wire 132
  CH_OHIPS_OFFSET_V1,			// wire 133
  CH_OHIPS_OFFSET_V1,			// wire 134
  CH_OHIPS_OFFSET_V1,			// wire 135
  CH_OHIPS_OFFSET_V1,			// wire 136
  CH_OHIPS_OFFSET_V1,			// wire 137
  CH_OHIPS_OFFSET_V1,			// wire 138
  CH_OHIPS_OFFSET_V1,			// wire 139
  CH_OHIPS_OFFSET_V1,			// wire 140
  CH_OHIPS_OFFSET_V1,			// wire 141
  CH_OHIPS_OFFSET_V1,			// wire 142
  CH_OHIPS_OFFSET_V1,			// wire 143
  CH_OHIPS_OFFSET_V1,			// wire 144
  CH_OHIPS_OFFSET_V1,			// wire 145
  CH_OHIPS_OFFSET_V1,			// wire 146
  CH_OHIPS_OFFSET_V1,			// wire 147
  CH_OHIPS_OFFSET_V1,			// wire 148
  CH_OHIPS_OFFSET_V1,			// wire 149
  CH_OHIPS_OFFSET_V1,			// wire 150
  CH_OHIPS_OFFSET_V1,			// wire 151
  CH_OHIPS_OFFSET_V1,			// wire 152
  CH_OHIPS_OFFSET_V1,			// wire 153
  CH_OHIPS_OFFSET_V1,			// wire 154
  CH_OHIPS_OFFSET_V1,			// wire 155
  CH_OHIPS_OFFSET_V1,			// wire 156
  CH_OHIPS_OFFSET_V1,			// wire 157
  CH_OHIPS_OFFSET_V1,			// wire 158
  CH_OHIPS_OFFSET_V1,			// wire 159
  CH_OHIPS_OFFSET_V1,			// wire 160
  CH_OHIPS_OFFSET_V1,			// wire 161
  CH_OHIPS_OFFSET_V1,			// wire 162
  CH_OHIPS_OFFSET_V1,			// wire 163
  CH_OHIPS_OFFSET_V1,			// wire 164
  CH_OHIPS_OFFSET_V1,			// wire 165
  CH_OHIPS_OFFSET_V1,			// wire 166
  CH_OHIPS_OFFSET_V1,			// wire 167
  CH_OHIPS_OFFSET_V1,			// wire 168
  CH_OHIPS_OFFSET_V1,			// wire 169
  CH_OHIPS_OFFSET_V1,			// wire 170
  CH_OHIPS_OFFSET_V1,			// wire 171
  CH_OHIPS_OFFSET_V1,			// wire 172
  CH_OHIPS_OFFSET_V1,			// wire 173
  CH_OHIPS_OFFSET_V1,			// wire 174
  CH_OHIPS_OFFSET_V1,			// wire 175
  CH_OHIPS_OFFSET_V1,			// wire 176
  CH_OHIPS_OFFSET_V1,			// wire 177
  CH_OHIPS_OFFSET_V1,			// wire 178
  CH_OHIPS_OFFSET_V1,			// wire 179
  CH_OHIPS_OFFSET_V1,			// wire 180
  CH_OHIPS_OFFSET_V1,			// wire 181
  CH_OHIPS_OFFSET_V1,			// wire 182
  CH_OHIPS_OFFSET_V1,			// wire 183
  CH_OHIPS_OFFSET_V1,			// wire 184
  CH_OHIPS_OFFSET_V1,			// wire 185
  CH_OHIPS_OFFSET_V1,			// wire 186
  CH_OHIPS_OFFSET_V1,			// wire 187
  CH_OHIPS_OFFSET_V1,			// wire 188
  CH_OHIPS_OFFSET_V1,			// wire 189
  CH_OHIPS_OFFSET_V1,			// wire 190
  CH_OHIPS_OFFSET_V1,			// wire 191
  CH_OHIPS_OFFSET_V1,			// wire 192
  CH_OHIPS_OFFSET_V1,			// wire 193
  CH_OHIPS_OFFSET_V1,			// wire 194
  CH_OHIPS_OFFSET_V1,			// wire 195
  CH_OHIPS_OFFSET_V1,			// wire 196
  CH_OHIPS_OFFSET_V1,			// wire 197
  CH_OHIPS_OFFSET_V1,			// wire 198
  CH_OHIPS_OFFSET_V1,			// wire 199
  CH_OHIPS_OFFSET_V1,			// wire 200
  CH_OHIPS_OFFSET_V1,			// wire 201
  CH_OHIPS_OFFSET_V1,			// wire 202
  CH_OHIPS_OFFSET_V1,			// wire 203
  CH_OHIPS_OFFSET_V1,			// wire 204
  CH_OHIPS_OFFSET_V1,			// wire 205
  CH_OHIPS_OFFSET_V1,			// wire 206
  CH_OHIPS_OFFSET_V1,			// wire 207
  CH_OHIPS_OFFSET_V1,			// wire 208
  CH_OHIPS_OFFSET_V1,			// wire 209
  CH_OHIPS_OFFSET_V1,			// wire 210
  CH_OHIPS_OFFSET_V1,			// wire 211
  CH_OHIPS_OFFSET_V1,			// wire 212
  CH_OHIPS_OFFSET_V1,			// wire 213
  CH_OHIPS_OFFSET_V1,			// wire 214
  CH_OHIPS_OFFSET_V1,			// wire 215
  CH_OHIPS_OFFSET_V1,			// wire 216
  CH_OHIPS_OFFSET_V1,			// wire 217
  CH_OHIPS_OFFSET_V1,			// wire 218
  CH_OHIPS_OFFSET_V1,			// wire 219
  CH_OHIPS_OFFSET_V1,			// wire 220
  CH_OHIPS_OFFSET_V1,			// wire 221
  CH_OHIPS_OFFSET_V1,			// wire 222
  CH_OHIPS_OFFSET_V1,			// wire 223
  CH_OHIPS_OFFSET_V1,			// wire 224
  CH_OHIPS_OFFSET_V1,			// wire 225
  CH_OHIPS_OFFSET_V1,			// wire 226
  CH_OHIPS_OFFSET_V1,			// wire 227
  CH_OHIPS_OFFSET_V1,			// wire 228
  CH_OHIPS_OFFSET_V1,			// wire 229
  CH_OHIPS_OFFSET_V1,			// wire 230
  CH_OHIPS_OFFSET_V1,			// wire 231
  CH_OHIPS_OFFSET_V1,			// wire 232
  CH_OHIPS_OFFSET_V1,			// wire 233
  CH_OHIPS_OFFSET_V1,			// wire 234
  CH_OHIPS_OFFSET_V1,			// wire 235
  CH_OHIPS_OFFSET_V1,			// wire 236
  CH_OHIPS_OFFSET_V1,			// wire 237
  CH_OHIPS_OFFSET_V1,			// wire 238
  CH_OHIPS_OFFSET_V1,			// wire 239
  CH_OHIPS_OFFSET_V1,			// wire 240
  CH_OHIPS_OFFSET_V1,			// wire 241
  CH_OHIPS_OFFSET_V1,			// wire 242
  CH_OHIPS_OFFSET_V1,			// wire 243
  CH_OHIPS_OFFSET_V1,			// wire 244
  CH_OHIPS_OFFSET_V1,			// wire 245
  CH_OHIPS_OFFSET_V1,			// wire 246
  CH_OHIPS_OFFSET_V1,			// wire 247
  CH_OHIPS_OFFSET_V1,			// wire 248
  CH_OHIPS_OFFSET_V1,			// wire 249
  CH_OHIPS_OFFSET_V1,			// wire 250
  CH_OHIPS_OFFSET_V1,			// wire 251
  CH_OHIPS_OFFSET_V1,			// wire 252
  CH_OHIPS_OFFSET_V1,			// wire 253
  CH_OHIPS_OFFSET_V1,			// wire 254
  CH_OHIPS_OFFSET_V1,			// wire 255
  CH_OHIPS_OFFSET_V1			// wire 256
};

static short
offset_ou2[CH_OHIPS_MAX_U2] = {
  CH_OHIPS_OFFSET_U2,			// wire 1
  CH_OHIPS_OFFSET_U2,			// wire 2
  CH_OHIPS_OFFSET_U2,			// wire 3
  CH_OHIPS_OFFSET_U2,			// wire 4
  CH_OHIPS_OFFSET_U2,			// wire 5
  CH_OHIPS_OFFSET_U2,			// wire 6
  CH_OHIPS_OFFSET_U2,			// wire 7
  CH_OHIPS_OFFSET_U2,			// wire 8
  CH_OHIPS_OFFSET_U2,			// wire 9
  CH_OHIPS_OFFSET_U2,			// wire 10
  CH_OHIPS_OFFSET_U2,			// wire 11
  CH_OHIPS_OFFSET_U2,			// wire 12
  CH_OHIPS_OFFSET_U2,			// wire 13
  CH_OHIPS_OFFSET_U2,			// wire 14
  CH_OHIPS_OFFSET_U2,			// wire 15
  CH_OHIPS_OFFSET_U2,			// wire 16
  CH_OHIPS_OFFSET_U2,			// wire 17
  CH_OHIPS_OFFSET_U2,			// wire 18
  CH_OHIPS_OFFSET_U2,			// wire 19
  CH_OHIPS_OFFSET_U2,			// wire 20
  CH_OHIPS_OFFSET_U2,			// wire 21
  CH_OHIPS_OFFSET_U2,			// wire 22
  CH_OHIPS_OFFSET_U2,			// wire 23
  CH_OHIPS_OFFSET_U2,			// wire 24
  CH_OHIPS_OFFSET_U2,			// wire 25
  CH_OHIPS_OFFSET_U2,			// wire 26
  CH_OHIPS_OFFSET_U2,			// wire 27
  CH_OHIPS_OFFSET_U2,			// wire 28
  CH_OHIPS_OFFSET_U2,			// wire 29
  CH_OHIPS_OFFSET_U2,			// wire 30
  CH_OHIPS_OFFSET_U2,			// wire 31
  CH_OHIPS_OFFSET_U2,			// wire 32
  CH_OHIPS_OFFSET_U2,			// wire 33
  CH_OHIPS_OFFSET_U2,			// wire 34
  CH_OHIPS_OFFSET_U2,			// wire 35
  CH_OHIPS_OFFSET_U2,			// wire 36
  CH_OHIPS_OFFSET_U2,			// wire 37
  CH_OHIPS_OFFSET_U2,			// wire 38
  CH_OHIPS_OFFSET_U2,			// wire 39
  CH_OHIPS_OFFSET_U2,			// wire 40
  CH_OHIPS_OFFSET_U2,			// wire 41
  CH_OHIPS_OFFSET_U2,			// wire 42
  CH_OHIPS_OFFSET_U2,			// wire 43
  CH_OHIPS_OFFSET_U2,			// wire 44
  CH_OHIPS_OFFSET_U2,			// wire 45
  CH_OHIPS_OFFSET_U2,			// wire 46
  CH_OHIPS_OFFSET_U2,			// wire 47
  CH_OHIPS_OFFSET_U2,			// wire 48
  CH_OHIPS_OFFSET_U2,			// wire 49
  CH_OHIPS_OFFSET_U2,			// wire 50
  CH_OHIPS_OFFSET_U2,			// wire 51
  CH_OHIPS_OFFSET_U2,			// wire 52
  CH_OHIPS_OFFSET_U2,			// wire 53
  CH_OHIPS_OFFSET_U2,			// wire 54
  CH_OHIPS_OFFSET_U2,			// wire 55
  CH_OHIPS_OFFSET_U2,			// wire 56
  CH_OHIPS_OFFSET_U2,			// wire 57
  CH_OHIPS_OFFSET_U2,			// wire 58
  CH_OHIPS_OFFSET_U2,			// wire 59
  CH_OHIPS_OFFSET_U2,			// wire 60
  CH_OHIPS_OFFSET_U2,			// wire 61
  CH_OHIPS_OFFSET_U2,			// wire 62
  CH_OHIPS_OFFSET_U2,			// wire 63
  CH_OHIPS_OFFSET_U2,			// wire 64
  CH_OHIPS_OFFSET_U2,			// wire 65
  CH_OHIPS_OFFSET_U2,			// wire 66
  CH_OHIPS_OFFSET_U2,			// wire 67
  CH_OHIPS_OFFSET_U2,			// wire 68
  CH_OHIPS_OFFSET_U2,			// wire 69
  CH_OHIPS_OFFSET_U2,			// wire 70
  CH_OHIPS_OFFSET_U2,			// wire 71
  CH_OHIPS_OFFSET_U2,			// wire 72
  CH_OHIPS_OFFSET_U2,			// wire 73
  CH_OHIPS_OFFSET_U2,			// wire 74
  CH_OHIPS_OFFSET_U2,			// wire 75
  CH_OHIPS_OFFSET_U2,			// wire 76
  CH_OHIPS_OFFSET_U2,			// wire 77
  CH_OHIPS_OFFSET_U2,			// wire 78
  CH_OHIPS_OFFSET_U2,			// wire 79
  CH_OHIPS_OFFSET_U2,			// wire 80
  CH_OHIPS_OFFSET_U2,			// wire 81
  CH_OHIPS_OFFSET_U2,			// wire 82
  CH_OHIPS_OFFSET_U2,			// wire 83
  CH_OHIPS_OFFSET_U2,			// wire 84
  CH_OHIPS_OFFSET_U2,			// wire 85
  CH_OHIPS_OFFSET_U2,			// wire 86
  CH_OHIPS_OFFSET_U2,			// wire 87
  CH_OHIPS_OFFSET_U2,			// wire 88
  CH_OHIPS_OFFSET_U2,			// wire 89
  CH_OHIPS_OFFSET_U2,			// wire 90
  CH_OHIPS_OFFSET_U2,			// wire 91
  CH_OHIPS_OFFSET_U2,			// wire 92
  CH_OHIPS_OFFSET_U2,			// wire 93
  CH_OHIPS_OFFSET_U2,			// wire 94
  CH_OHIPS_OFFSET_U2,			// wire 95
  CH_OHIPS_OFFSET_U2,			// wire 96
  CH_OHIPS_OFFSET_U2,			// wire 97
  CH_OHIPS_OFFSET_U2,			// wire 98
  CH_OHIPS_OFFSET_U2,			// wire 99
  CH_OHIPS_OFFSET_U2,			// wire 100
  CH_OHIPS_OFFSET_U2,			// wire 101
  CH_OHIPS_OFFSET_U2,			// wire 102
  CH_OHIPS_OFFSET_U2,			// wire 103
  CH_OHIPS_OFFSET_U2,			// wire 104
  CH_OHIPS_OFFSET_U2,			// wire 105
  CH_OHIPS_OFFSET_U2,			// wire 106
  CH_OHIPS_OFFSET_U2,			// wire 107
  CH_OHIPS_OFFSET_U2,			// wire 108
  CH_OHIPS_OFFSET_U2,			// wire 109
  CH_OHIPS_OFFSET_U2,			// wire 110
  CH_OHIPS_OFFSET_U2,			// wire 111
  CH_OHIPS_OFFSET_U2,			// wire 112
  CH_OHIPS_OFFSET_U2,			// wire 113
  CH_OHIPS_OFFSET_U2,			// wire 114
  CH_OHIPS_OFFSET_U2,			// wire 115
  CH_OHIPS_OFFSET_U2,			// wire 116
  CH_OHIPS_OFFSET_U2,			// wire 117
  CH_OHIPS_OFFSET_U2,			// wire 118
  CH_OHIPS_OFFSET_U2,			// wire 119
  CH_OHIPS_OFFSET_U2,			// wire 120
  CH_OHIPS_OFFSET_U2,			// wire 121
  CH_OHIPS_OFFSET_U2,			// wire 122
  CH_OHIPS_OFFSET_U2,			// wire 123
  CH_OHIPS_OFFSET_U2,			// wire 124
  CH_OHIPS_OFFSET_U2,			// wire 125
  CH_OHIPS_OFFSET_U2,			// wire 126
  CH_OHIPS_OFFSET_U2,			// wire 127
  CH_OHIPS_OFFSET_U2,			// wire 128
  CH_OHIPS_OFFSET_U2,			// wire 129
  CH_OHIPS_OFFSET_U2,			// wire 130
  CH_OHIPS_OFFSET_U2,			// wire 131
  CH_OHIPS_OFFSET_U2,			// wire 132
  CH_OHIPS_OFFSET_U2,			// wire 133
  CH_OHIPS_OFFSET_U2,			// wire 134
  CH_OHIPS_OFFSET_U2,			// wire 135
  CH_OHIPS_OFFSET_U2,			// wire 136
  CH_OHIPS_OFFSET_U2,			// wire 137
  CH_OHIPS_OFFSET_U2,			// wire 138
  CH_OHIPS_OFFSET_U2,			// wire 139
  CH_OHIPS_OFFSET_U2,			// wire 140
  CH_OHIPS_OFFSET_U2,			// wire 141
  CH_OHIPS_OFFSET_U2,			// wire 142
  CH_OHIPS_OFFSET_U2,			// wire 143
  CH_OHIPS_OFFSET_U2,			// wire 144
  CH_OHIPS_OFFSET_U2,			// wire 145
  CH_OHIPS_OFFSET_U2,			// wire 146
  CH_OHIPS_OFFSET_U2,			// wire 147
  CH_OHIPS_OFFSET_U2,			// wire 148
  CH_OHIPS_OFFSET_U2,			// wire 149
  CH_OHIPS_OFFSET_U2,			// wire 150
  CH_OHIPS_OFFSET_U2,			// wire 151
  CH_OHIPS_OFFSET_U2,			// wire 152
  CH_OHIPS_OFFSET_U2,			// wire 153
  CH_OHIPS_OFFSET_U2,			// wire 154
  CH_OHIPS_OFFSET_U2,			// wire 155
  CH_OHIPS_OFFSET_U2,			// wire 156
  CH_OHIPS_OFFSET_U2,			// wire 157
  CH_OHIPS_OFFSET_U2,			// wire 158
  CH_OHIPS_OFFSET_U2,			// wire 159
  CH_OHIPS_OFFSET_U2,			// wire 160
  CH_OHIPS_OFFSET_U2,			// wire 161
  CH_OHIPS_OFFSET_U2,			// wire 162
  CH_OHIPS_OFFSET_U2,			// wire 163
  CH_OHIPS_OFFSET_U2,			// wire 164
  CH_OHIPS_OFFSET_U2,			// wire 165
  CH_OHIPS_OFFSET_U2,			// wire 166
  CH_OHIPS_OFFSET_U2,			// wire 167
  CH_OHIPS_OFFSET_U2,			// wire 168
  CH_OHIPS_OFFSET_U2,			// wire 169
  CH_OHIPS_OFFSET_U2,			// wire 170
  CH_OHIPS_OFFSET_U2,			// wire 171
  CH_OHIPS_OFFSET_U2,			// wire 172
  CH_OHIPS_OFFSET_U2,			// wire 173
  CH_OHIPS_OFFSET_U2,			// wire 174
  CH_OHIPS_OFFSET_U2,			// wire 175
  CH_OHIPS_OFFSET_U2,			// wire 176
  CH_OHIPS_OFFSET_U2,			// wire 177
  CH_OHIPS_OFFSET_U2,			// wire 178
  CH_OHIPS_OFFSET_U2,			// wire 179
  CH_OHIPS_OFFSET_U2,			// wire 180
  CH_OHIPS_OFFSET_U2,			// wire 181
  CH_OHIPS_OFFSET_U2,			// wire 182
  CH_OHIPS_OFFSET_U2,			// wire 183
  CH_OHIPS_OFFSET_U2,			// wire 184
  CH_OHIPS_OFFSET_U2,			// wire 185
  CH_OHIPS_OFFSET_U2,			// wire 186
  CH_OHIPS_OFFSET_U2,			// wire 187
  CH_OHIPS_OFFSET_U2,			// wire 188
  CH_OHIPS_OFFSET_U2,			// wire 189
  CH_OHIPS_OFFSET_U2,			// wire 190
  CH_OHIPS_OFFSET_U2,			// wire 191
  CH_OHIPS_OFFSET_U2,			// wire 192
  CH_OHIPS_OFFSET_U2,			// wire 193
  CH_OHIPS_OFFSET_U2,			// wire 194
  CH_OHIPS_OFFSET_U2,			// wire 195
  CH_OHIPS_OFFSET_U2,			// wire 196
  CH_OHIPS_OFFSET_U2,			// wire 197
  CH_OHIPS_OFFSET_U2,			// wire 198
  CH_OHIPS_OFFSET_U2,			// wire 199
  CH_OHIPS_OFFSET_U2,			// wire 200
  CH_OHIPS_OFFSET_U2,			// wire 201
  CH_OHIPS_OFFSET_U2,			// wire 202
  CH_OHIPS_OFFSET_U2,			// wire 203
  CH_OHIPS_OFFSET_U2,			// wire 204
  CH_OHIPS_OFFSET_U2,			// wire 205
  CH_OHIPS_OFFSET_U2,			// wire 206
  CH_OHIPS_OFFSET_U2,			// wire 207
  CH_OHIPS_OFFSET_U2,			// wire 208
  CH_OHIPS_OFFSET_U2,			// wire 209
  CH_OHIPS_OFFSET_U2,			// wire 210
  CH_OHIPS_OFFSET_U2,			// wire 211
  CH_OHIPS_OFFSET_U2,			// wire 212
  CH_OHIPS_OFFSET_U2,			// wire 213
  CH_OHIPS_OFFSET_U2,			// wire 214
  CH_OHIPS_OFFSET_U2,			// wire 215
  CH_OHIPS_OFFSET_U2,			// wire 216
  CH_OHIPS_OFFSET_U2,			// wire 217
  CH_OHIPS_OFFSET_U2,			// wire 218
  CH_OHIPS_OFFSET_U2,			// wire 219
  CH_OHIPS_OFFSET_U2,			// wire 220
  CH_OHIPS_OFFSET_U2,			// wire 221
  CH_OHIPS_OFFSET_U2,			// wire 222
  CH_OHIPS_OFFSET_U2,			// wire 223
  CH_OHIPS_OFFSET_U2,			// wire 224
  CH_OHIPS_OFFSET_U2,			// wire 225
  CH_OHIPS_OFFSET_U2,			// wire 226
  CH_OHIPS_OFFSET_U2,			// wire 227
  CH_OHIPS_OFFSET_U2,			// wire 228
  CH_OHIPS_OFFSET_U2,			// wire 229
  CH_OHIPS_OFFSET_U2,			// wire 230
  CH_OHIPS_OFFSET_U2,			// wire 231
  CH_OHIPS_OFFSET_U2,			// wire 232
  CH_OHIPS_OFFSET_U2,			// wire 233
  CH_OHIPS_OFFSET_U2,			// wire 234
  CH_OHIPS_OFFSET_U2,			// wire 235
  CH_OHIPS_OFFSET_U2,			// wire 236
  CH_OHIPS_OFFSET_U2,			// wire 237
  CH_OHIPS_OFFSET_U2,			// wire 238
  CH_OHIPS_OFFSET_U2,			// wire 239
  CH_OHIPS_OFFSET_U2,			// wire 240
  CH_OHIPS_OFFSET_U2,			// wire 241
  CH_OHIPS_OFFSET_U2,			// wire 242
  CH_OHIPS_OFFSET_U2,			// wire 243
  CH_OHIPS_OFFSET_U2,			// wire 244
  CH_OHIPS_OFFSET_U2,			// wire 245
  CH_OHIPS_OFFSET_U2,			// wire 246
  CH_OHIPS_OFFSET_U2,			// wire 247
  CH_OHIPS_OFFSET_U2,			// wire 248
  CH_OHIPS_OFFSET_U2,			// wire 249
  CH_OHIPS_OFFSET_U2,			// wire 250
  CH_OHIPS_OFFSET_U2,			// wire 251
  CH_OHIPS_OFFSET_U2,			// wire 252
  CH_OHIPS_OFFSET_U2,			// wire 253
  CH_OHIPS_OFFSET_U2,			// wire 254
  CH_OHIPS_OFFSET_U2,			// wire 255
  CH_OHIPS_OFFSET_U2			// wire 256
};

static short
offset_ov2[CH_OHIPS_MAX_V2] = {
  CH_OHIPS_OFFSET_V2,			// wire 1
  CH_OHIPS_OFFSET_V2,			// wire 2
  CH_OHIPS_OFFSET_V2,			// wire 3
  CH_OHIPS_OFFSET_V2,			// wire 4
  CH_OHIPS_OFFSET_V2,			// wire 5
  CH_OHIPS_OFFSET_V2,			// wire 6
  CH_OHIPS_OFFSET_V2,			// wire 7
  CH_OHIPS_OFFSET_V2,			// wire 8
  CH_OHIPS_OFFSET_V2,			// wire 9
  CH_OHIPS_OFFSET_V2,			// wire 10
  CH_OHIPS_OFFSET_V2,			// wire 11
  CH_OHIPS_OFFSET_V2,			// wire 12
  CH_OHIPS_OFFSET_V2,			// wire 13
  CH_OHIPS_OFFSET_V2,			// wire 14
  CH_OHIPS_OFFSET_V2,			// wire 15
  CH_OHIPS_OFFSET_V2,			// wire 16
  CH_OHIPS_OFFSET_V2,			// wire 17
  CH_OHIPS_OFFSET_V2,			// wire 18
  CH_OHIPS_OFFSET_V2,			// wire 19
  CH_OHIPS_OFFSET_V2,			// wire 20
  CH_OHIPS_OFFSET_V2,			// wire 21
  CH_OHIPS_OFFSET_V2,			// wire 22
  CH_OHIPS_OFFSET_V2,			// wire 23
  CH_OHIPS_OFFSET_V2,			// wire 24
  CH_OHIPS_OFFSET_V2,			// wire 25
  CH_OHIPS_OFFSET_V2,			// wire 26
  CH_OHIPS_OFFSET_V2,			// wire 27
  CH_OHIPS_OFFSET_V2,			// wire 28
  CH_OHIPS_OFFSET_V2,			// wire 29
  CH_OHIPS_OFFSET_V2,			// wire 30
  CH_OHIPS_OFFSET_V2,			// wire 31
  CH_OHIPS_OFFSET_V2,			// wire 32
  CH_OHIPS_OFFSET_V2,			// wire 33
  CH_OHIPS_OFFSET_V2,			// wire 34
  CH_OHIPS_OFFSET_V2,			// wire 35
  CH_OHIPS_OFFSET_V2,			// wire 36
  CH_OHIPS_OFFSET_V2,			// wire 37
  CH_OHIPS_OFFSET_V2,			// wire 38
  CH_OHIPS_OFFSET_V2,			// wire 39
  CH_OHIPS_OFFSET_V2,			// wire 40
  CH_OHIPS_OFFSET_V2,			// wire 41
  CH_OHIPS_OFFSET_V2,			// wire 42
  CH_OHIPS_OFFSET_V2,			// wire 43
  CH_OHIPS_OFFSET_V2,			// wire 44
  CH_OHIPS_OFFSET_V2,			// wire 45
  CH_OHIPS_OFFSET_V2,			// wire 46
  CH_OHIPS_OFFSET_V2,			// wire 47
  CH_OHIPS_OFFSET_V2,			// wire 48
  CH_OHIPS_OFFSET_V2,			// wire 49
  CH_OHIPS_OFFSET_V2,			// wire 50
  CH_OHIPS_OFFSET_V2,			// wire 51
  CH_OHIPS_OFFSET_V2,			// wire 52
  CH_OHIPS_OFFSET_V2,			// wire 53
  CH_OHIPS_OFFSET_V2,			// wire 54
  CH_OHIPS_OFFSET_V2,			// wire 55
  CH_OHIPS_OFFSET_V2,			// wire 56
  CH_OHIPS_OFFSET_V2,			// wire 57
  CH_OHIPS_OFFSET_V2,			// wire 58
  CH_OHIPS_OFFSET_V2,			// wire 59
  CH_OHIPS_OFFSET_V2,			// wire 60
  CH_OHIPS_OFFSET_V2,			// wire 61
  CH_OHIPS_OFFSET_V2,			// wire 62
  CH_OHIPS_OFFSET_V2,			// wire 63
  CH_OHIPS_OFFSET_V2,			// wire 64
  CH_OHIPS_OFFSET_V2,			// wire 65
  CH_OHIPS_OFFSET_V2,			// wire 66
  CH_OHIPS_OFFSET_V2,			// wire 67
  CH_OHIPS_OFFSET_V2,			// wire 68
  CH_OHIPS_OFFSET_V2,			// wire 69
  CH_OHIPS_OFFSET_V2,			// wire 70
  CH_OHIPS_OFFSET_V2,			// wire 71
  CH_OHIPS_OFFSET_V2,			// wire 72
  CH_OHIPS_OFFSET_V2,			// wire 73
  CH_OHIPS_OFFSET_V2,			// wire 74
  CH_OHIPS_OFFSET_V2,			// wire 75
  CH_OHIPS_OFFSET_V2,			// wire 76
  CH_OHIPS_OFFSET_V2,			// wire 77
  CH_OHIPS_OFFSET_V2,			// wire 78
  CH_OHIPS_OFFSET_V2,			// wire 79
  CH_OHIPS_OFFSET_V2,			// wire 80
  CH_OHIPS_OFFSET_V2,			// wire 81
  CH_OHIPS_OFFSET_V2,			// wire 82
  CH_OHIPS_OFFSET_V2,			// wire 83
  CH_OHIPS_OFFSET_V2,			// wire 84
  CH_OHIPS_OFFSET_V2,			// wire 85
  CH_OHIPS_OFFSET_V2,			// wire 86
  CH_OHIPS_OFFSET_V2,			// wire 87
  CH_OHIPS_OFFSET_V2,			// wire 88
  CH_OHIPS_OFFSET_V2,			// wire 89
  CH_OHIPS_OFFSET_V2,			// wire 90
  CH_OHIPS_OFFSET_V2,			// wire 91
  CH_OHIPS_OFFSET_V2,			// wire 92
  CH_OHIPS_OFFSET_V2,			// wire 93
  CH_OHIPS_OFFSET_V2,			// wire 94
  CH_OHIPS_OFFSET_V2,			// wire 95
  CH_OHIPS_OFFSET_V2,			// wire 96
  CH_OHIPS_OFFSET_V2,			// wire 97
  CH_OHIPS_OFFSET_V2,			// wire 98
  CH_OHIPS_OFFSET_V2,			// wire 99
  CH_OHIPS_OFFSET_V2,			// wire 100
  CH_OHIPS_OFFSET_V2,			// wire 101
  CH_OHIPS_OFFSET_V2,			// wire 102
  CH_OHIPS_OFFSET_V2,			// wire 103
  CH_OHIPS_OFFSET_V2,			// wire 104
  CH_OHIPS_OFFSET_V2,			// wire 105
  CH_OHIPS_OFFSET_V2,			// wire 106
  CH_OHIPS_OFFSET_V2,			// wire 107
  CH_OHIPS_OFFSET_V2,			// wire 108
  CH_OHIPS_OFFSET_V2,			// wire 109
  CH_OHIPS_OFFSET_V2,			// wire 110
  CH_OHIPS_OFFSET_V2,			// wire 111
  CH_OHIPS_OFFSET_V2,			// wire 112
  CH_OHIPS_OFFSET_V2,			// wire 113
  CH_OHIPS_OFFSET_V2,			// wire 114
  CH_OHIPS_OFFSET_V2,			// wire 115
  CH_OHIPS_OFFSET_V2,			// wire 116
  CH_OHIPS_OFFSET_V2,			// wire 117
  CH_OHIPS_OFFSET_V2,			// wire 118
  CH_OHIPS_OFFSET_V2,			// wire 119
  CH_OHIPS_OFFSET_V2,			// wire 120
  CH_OHIPS_OFFSET_V2,			// wire 121
  CH_OHIPS_OFFSET_V2,			// wire 122
  CH_OHIPS_OFFSET_V2,			// wire 123
  CH_OHIPS_OFFSET_V2,			// wire 124
  CH_OHIPS_OFFSET_V2,			// wire 125
  CH_OHIPS_OFFSET_V2,			// wire 126
  CH_OHIPS_OFFSET_V2,			// wire 127
  CH_OHIPS_OFFSET_V2,			// wire 128
  CH_OHIPS_OFFSET_V2,			// wire 129
  CH_OHIPS_OFFSET_V2,			// wire 130
  CH_OHIPS_OFFSET_V2,			// wire 131
  CH_OHIPS_OFFSET_V2,			// wire 132
  CH_OHIPS_OFFSET_V2,			// wire 133
  CH_OHIPS_OFFSET_V2,			// wire 134
  CH_OHIPS_OFFSET_V2,			// wire 135
  CH_OHIPS_OFFSET_V2,			// wire 136
  CH_OHIPS_OFFSET_V2,			// wire 137
  CH_OHIPS_OFFSET_V2,			// wire 138
  CH_OHIPS_OFFSET_V2,			// wire 139
  CH_OHIPS_OFFSET_V2,			// wire 140
  CH_OHIPS_OFFSET_V2,			// wire 141
  CH_OHIPS_OFFSET_V2,			// wire 142
  CH_OHIPS_OFFSET_V2,			// wire 143
  CH_OHIPS_OFFSET_V2,			// wire 144
  CH_OHIPS_OFFSET_V2,			// wire 145
  CH_OHIPS_OFFSET_V2,			// wire 146
  CH_OHIPS_OFFSET_V2,			// wire 147
  CH_OHIPS_OFFSET_V2,			// wire 148
  CH_OHIPS_OFFSET_V2,			// wire 149
  CH_OHIPS_OFFSET_V2,			// wire 150
  CH_OHIPS_OFFSET_V2,			// wire 151
  CH_OHIPS_OFFSET_V2,			// wire 152
  CH_OHIPS_OFFSET_V2,			// wire 153
  CH_OHIPS_OFFSET_V2,			// wire 154
  CH_OHIPS_OFFSET_V2,			// wire 155
  CH_OHIPS_OFFSET_V2,			// wire 156
  CH_OHIPS_OFFSET_V2,			// wire 157
  CH_OHIPS_OFFSET_V2,			// wire 158
  CH_OHIPS_OFFSET_V2,			// wire 159
  CH_OHIPS_OFFSET_V2,			// wire 160
  CH_OHIPS_OFFSET_V2,			// wire 161
  CH_OHIPS_OFFSET_V2,			// wire 162
  CH_OHIPS_OFFSET_V2,			// wire 163
  CH_OHIPS_OFFSET_V2,			// wire 164
  CH_OHIPS_OFFSET_V2,			// wire 165
  CH_OHIPS_OFFSET_V2,			// wire 166
  CH_OHIPS_OFFSET_V2,			// wire 167
  CH_OHIPS_OFFSET_V2,			// wire 168
  CH_OHIPS_OFFSET_V2,			// wire 169
  CH_OHIPS_OFFSET_V2,			// wire 170
  CH_OHIPS_OFFSET_V2,			// wire 171
  CH_OHIPS_OFFSET_V2,			// wire 172
  CH_OHIPS_OFFSET_V2,			// wire 173
  CH_OHIPS_OFFSET_V2,			// wire 174
  CH_OHIPS_OFFSET_V2,			// wire 175
  CH_OHIPS_OFFSET_V2,			// wire 176
  CH_OHIPS_OFFSET_V2,			// wire 177
  CH_OHIPS_OFFSET_V2,			// wire 178
  CH_OHIPS_OFFSET_V2,			// wire 179
  CH_OHIPS_OFFSET_V2,			// wire 180
  CH_OHIPS_OFFSET_V2,			// wire 181
  CH_OHIPS_OFFSET_V2,			// wire 182
  CH_OHIPS_OFFSET_V2,			// wire 183
  CH_OHIPS_OFFSET_V2,			// wire 184
  CH_OHIPS_OFFSET_V2,			// wire 185
  CH_OHIPS_OFFSET_V2,			// wire 186
  CH_OHIPS_OFFSET_V2,			// wire 187
  CH_OHIPS_OFFSET_V2,			// wire 188
  CH_OHIPS_OFFSET_V2,			// wire 189
  CH_OHIPS_OFFSET_V2,			// wire 190
  CH_OHIPS_OFFSET_V2,			// wire 191
  CH_OHIPS_OFFSET_V2,			// wire 192
  CH_OHIPS_OFFSET_V2,			// wire 193
  CH_OHIPS_OFFSET_V2,			// wire 194
  CH_OHIPS_OFFSET_V2,			// wire 195
  CH_OHIPS_OFFSET_V2,			// wire 196
  CH_OHIPS_OFFSET_V2,			// wire 197
  CH_OHIPS_OFFSET_V2,			// wire 198
  CH_OHIPS_OFFSET_V2,			// wire 199
  CH_OHIPS_OFFSET_V2,			// wire 200
  CH_OHIPS_OFFSET_V2,			// wire 201
  CH_OHIPS_OFFSET_V2,			// wire 202
  CH_OHIPS_OFFSET_V2,			// wire 203
  CH_OHIPS_OFFSET_V2,			// wire 204
  CH_OHIPS_OFFSET_V2,			// wire 205
  CH_OHIPS_OFFSET_V2,			// wire 206
  CH_OHIPS_OFFSET_V2,			// wire 207
  CH_OHIPS_OFFSET_V2,			// wire 208
  CH_OHIPS_OFFSET_V2,			// wire 209
  CH_OHIPS_OFFSET_V2,			// wire 210
  CH_OHIPS_OFFSET_V2,			// wire 211
  CH_OHIPS_OFFSET_V2,			// wire 212
  CH_OHIPS_OFFSET_V2,			// wire 213
  CH_OHIPS_OFFSET_V2,			// wire 214
  CH_OHIPS_OFFSET_V2,			// wire 215
  CH_OHIPS_OFFSET_V2,			// wire 216
  CH_OHIPS_OFFSET_V2,			// wire 217
  CH_OHIPS_OFFSET_V2,			// wire 218
  CH_OHIPS_OFFSET_V2,			// wire 219
  CH_OHIPS_OFFSET_V2,			// wire 220
  CH_OHIPS_OFFSET_V2,			// wire 221
  CH_OHIPS_OFFSET_V2,			// wire 222
  CH_OHIPS_OFFSET_V2,			// wire 223
  CH_OHIPS_OFFSET_V2,			// wire 224
  CH_OHIPS_OFFSET_V2,			// wire 225
  CH_OHIPS_OFFSET_V2,			// wire 226
  CH_OHIPS_OFFSET_V2,			// wire 227
  CH_OHIPS_OFFSET_V2,			// wire 228
  CH_OHIPS_OFFSET_V2,			// wire 229
  CH_OHIPS_OFFSET_V2,			// wire 230
  CH_OHIPS_OFFSET_V2,			// wire 231
  CH_OHIPS_OFFSET_V2,			// wire 232
  CH_OHIPS_OFFSET_V2,			// wire 233
  CH_OHIPS_OFFSET_V2,			// wire 234
  CH_OHIPS_OFFSET_V2,			// wire 235
  CH_OHIPS_OFFSET_V2,			// wire 236
  CH_OHIPS_OFFSET_V2,			// wire 237
  CH_OHIPS_OFFSET_V2,			// wire 238
  CH_OHIPS_OFFSET_V2,			// wire 239
  CH_OHIPS_OFFSET_V2,			// wire 240
  CH_OHIPS_OFFSET_V2,			// wire 241
  CH_OHIPS_OFFSET_V2,			// wire 242
  CH_OHIPS_OFFSET_V2,			// wire 243
  CH_OHIPS_OFFSET_V2,			// wire 244
  CH_OHIPS_OFFSET_V2,			// wire 245
  CH_OHIPS_OFFSET_V2,			// wire 246
  CH_OHIPS_OFFSET_V2,			// wire 247
  CH_OHIPS_OFFSET_V2,			// wire 248
  CH_OHIPS_OFFSET_V2,			// wire 249
  CH_OHIPS_OFFSET_V2,			// wire 250
  CH_OHIPS_OFFSET_V2,			// wire 251
  CH_OHIPS_OFFSET_V2,			// wire 252
  CH_OHIPS_OFFSET_V2,			// wire 253
  CH_OHIPS_OFFSET_V2,			// wire 254
  CH_OHIPS_OFFSET_V2,			// wire 255
  CH_OHIPS_OFFSET_V2			// wire 256
};

static float
position_ou1[CH_OHIPS_MAX_U1] = {
  CH_OHIPS_POSITION_U1+1.*25.4/sqrt(32.0),	// u1 wire 1
  CH_OHIPS_POSITION_U1+2.*25.4/sqrt(32.0),	// u1 wire 2
  CH_OHIPS_POSITION_U1+3.*25.4/sqrt(32.0),	// u1 wire 3
  CH_OHIPS_POSITION_U1+4.*25.4/sqrt(32.0),	// u1 wire 4
  CH_OHIPS_POSITION_U1+5.*25.4/sqrt(32.0),	// u1 wire 5
  CH_OHIPS_POSITION_U1+6.*25.4/sqrt(32.0),	// u1 wire 6
  CH_OHIPS_POSITION_U1+7.*25.4/sqrt(32.0),	// u1 wire 7
  CH_OHIPS_POSITION_U1+8.*25.4/sqrt(32.0),	// u1 wire 8
  CH_OHIPS_POSITION_U1+9.*25.4/sqrt(32.0),	// u1 wire 9
  CH_OHIPS_POSITION_U1+10.*25.4/sqrt(32.0),	// u1 wire 10
  CH_OHIPS_POSITION_U1+11.*25.4/sqrt(32.0),	// u1 wire 11
  CH_OHIPS_POSITION_U1+12.*25.4/sqrt(32.0),	// u1 wire 12
  CH_OHIPS_POSITION_U1+13.*25.4/sqrt(32.0),	// u1 wire 13
  CH_OHIPS_POSITION_U1+14.*25.4/sqrt(32.0),	// u1 wire 14
  CH_OHIPS_POSITION_U1+15.*25.4/sqrt(32.0),	// u1 wire 15
  CH_OHIPS_POSITION_U1+16.*25.4/sqrt(32.0),	// u1 wire 16
  CH_OHIPS_POSITION_U1+17.*25.4/sqrt(32.0),	// u1 wire 17
  CH_OHIPS_POSITION_U1+18.*25.4/sqrt(32.0),	// u1 wire 18
  CH_OHIPS_POSITION_U1+19.*25.4/sqrt(32.0),	// u1 wire 19
  CH_OHIPS_POSITION_U1+20.*25.4/sqrt(32.0),	// u1 wire 20
  CH_OHIPS_POSITION_U1+21.*25.4/sqrt(32.0),	// u1 wire 21
  CH_OHIPS_POSITION_U1+22.*25.4/sqrt(32.0),	// u1 wire 22
  CH_OHIPS_POSITION_U1+23.*25.4/sqrt(32.0),	// u1 wire 23
  CH_OHIPS_POSITION_U1+24.*25.4/sqrt(32.0),	// u1 wire 24
  CH_OHIPS_POSITION_U1+25.*25.4/sqrt(32.0),	// u1 wire 25
  CH_OHIPS_POSITION_U1+26.*25.4/sqrt(32.0),	// u1 wire 26
  CH_OHIPS_POSITION_U1+27.*25.4/sqrt(32.0),	// u1 wire 27
  CH_OHIPS_POSITION_U1+28.*25.4/sqrt(32.0),	// u1 wire 28
  CH_OHIPS_POSITION_U1+29.*25.4/sqrt(32.0),	// u1 wire 29
  CH_OHIPS_POSITION_U1+30.*25.4/sqrt(32.0),	// u1 wire 30
  CH_OHIPS_POSITION_U1+31.*25.4/sqrt(32.0),	// u1 wire 31
  CH_OHIPS_POSITION_U1+32.*25.4/sqrt(32.0),	// u1 wire 32
  CH_OHIPS_POSITION_U1+33.*25.4/sqrt(32.0),	// u1 wire 33
  CH_OHIPS_POSITION_U1+34.*25.4/sqrt(32.0),	// u1 wire 34
  CH_OHIPS_POSITION_U1+35.*25.4/sqrt(32.0),	// u1 wire 35
  CH_OHIPS_POSITION_U1+36.*25.4/sqrt(32.0),	// u1 wire 36
  CH_OHIPS_POSITION_U1+37.*25.4/sqrt(32.0),	// u1 wire 37
  CH_OHIPS_POSITION_U1+38.*25.4/sqrt(32.0),	// u1 wire 38
  CH_OHIPS_POSITION_U1+39.*25.4/sqrt(32.0),	// u1 wire 39
  CH_OHIPS_POSITION_U1+40.*25.4/sqrt(32.0),	// u1 wire 40
  CH_OHIPS_POSITION_U1+41.*25.4/sqrt(32.0),	// u1 wire 41
  CH_OHIPS_POSITION_U1+42.*25.4/sqrt(32.0),	// u1 wire 42
  CH_OHIPS_POSITION_U1+43.*25.4/sqrt(32.0),	// u1 wire 43
  CH_OHIPS_POSITION_U1+44.*25.4/sqrt(32.0),	// u1 wire 44
  CH_OHIPS_POSITION_U1+45.*25.4/sqrt(32.0),	// u1 wire 45
  CH_OHIPS_POSITION_U1+46.*25.4/sqrt(32.0),	// u1 wire 46
  CH_OHIPS_POSITION_U1+47.*25.4/sqrt(32.0),	// u1 wire 47
  CH_OHIPS_POSITION_U1+48.*25.4/sqrt(32.0),	// u1 wire 48
  CH_OHIPS_POSITION_U1+49.*25.4/sqrt(32.0),	// u1 wire 49
  CH_OHIPS_POSITION_U1+50.*25.4/sqrt(32.0),	// u1 wire 50
  CH_OHIPS_POSITION_U1+51.*25.4/sqrt(32.0),	// u1 wire 51
  CH_OHIPS_POSITION_U1+52.*25.4/sqrt(32.0),	// u1 wire 52
  CH_OHIPS_POSITION_U1+53.*25.4/sqrt(32.0),	// u1 wire 53
  CH_OHIPS_POSITION_U1+54.*25.4/sqrt(32.0),	// u1 wire 54
  CH_OHIPS_POSITION_U1+55.*25.4/sqrt(32.0),	// u1 wire 55
  CH_OHIPS_POSITION_U1+56.*25.4/sqrt(32.0),	// u1 wire 56
  CH_OHIPS_POSITION_U1+57.*25.4/sqrt(32.0),	// u1 wire 57
  CH_OHIPS_POSITION_U1+58.*25.4/sqrt(32.0),	// u1 wire 58
  CH_OHIPS_POSITION_U1+59.*25.4/sqrt(32.0),	// u1 wire 59
  CH_OHIPS_POSITION_U1+60.*25.4/sqrt(32.0),	// u1 wire 60
  CH_OHIPS_POSITION_U1+61.*25.4/sqrt(32.0),	// u1 wire 61
  CH_OHIPS_POSITION_U1+62.*25.4/sqrt(32.0),	// u1 wire 62
  CH_OHIPS_POSITION_U1+63.*25.4/sqrt(32.0),	// u1 wire 63
  CH_OHIPS_POSITION_U1+64.*25.4/sqrt(32.0),	// u1 wire 64
  CH_OHIPS_POSITION_U1+65.*25.4/sqrt(32.0),	// u1 wire 65
  CH_OHIPS_POSITION_U1+66.*25.4/sqrt(32.0),	// u1 wire 66
  CH_OHIPS_POSITION_U1+67.*25.4/sqrt(32.0),	// u1 wire 67
  CH_OHIPS_POSITION_U1+68.*25.4/sqrt(32.0),	// u1 wire 68
  CH_OHIPS_POSITION_U1+69.*25.4/sqrt(32.0),	// u1 wire 69
  CH_OHIPS_POSITION_U1+70.*25.4/sqrt(32.0),	// u1 wire 70
  CH_OHIPS_POSITION_U1+71.*25.4/sqrt(32.0),	// u1 wire 71
  CH_OHIPS_POSITION_U1+72.*25.4/sqrt(32.0),	// u1 wire 72
  CH_OHIPS_POSITION_U1+73.*25.4/sqrt(32.0),	// u1 wire 73
  CH_OHIPS_POSITION_U1+74.*25.4/sqrt(32.0),	// u1 wire 74
  CH_OHIPS_POSITION_U1+75.*25.4/sqrt(32.0),	// u1 wire 75
  CH_OHIPS_POSITION_U1+76.*25.4/sqrt(32.0),	// u1 wire 76
  CH_OHIPS_POSITION_U1+77.*25.4/sqrt(32.0),	// u1 wire 77
  CH_OHIPS_POSITION_U1+78.*25.4/sqrt(32.0),	// u1 wire 78
  CH_OHIPS_POSITION_U1+79.*25.4/sqrt(32.0),	// u1 wire 79
  CH_OHIPS_POSITION_U1+80.*25.4/sqrt(32.0),	// u1 wire 80
  CH_OHIPS_POSITION_U1+81.*25.4/sqrt(32.0),	// u1 wire 81
  CH_OHIPS_POSITION_U1+82.*25.4/sqrt(32.0),	// u1 wire 82
  CH_OHIPS_POSITION_U1+83.*25.4/sqrt(32.0),	// u1 wire 83
  CH_OHIPS_POSITION_U1+84.*25.4/sqrt(32.0),	// u1 wire 84
  CH_OHIPS_POSITION_U1+85.*25.4/sqrt(32.0),	// u1 wire 85
  CH_OHIPS_POSITION_U1+86.*25.4/sqrt(32.0),	// u1 wire 86
  CH_OHIPS_POSITION_U1+87.*25.4/sqrt(32.0),	// u1 wire 87
  CH_OHIPS_POSITION_U1+88.*25.4/sqrt(32.0),	// u1 wire 88
  CH_OHIPS_POSITION_U1+89.*25.4/sqrt(32.0),	// u1 wire 89
  CH_OHIPS_POSITION_U1+90.*25.4/sqrt(32.0),	// u1 wire 90
  CH_OHIPS_POSITION_U1+91.*25.4/sqrt(32.0),	// u1 wire 91
  CH_OHIPS_POSITION_U1+92.*25.4/sqrt(32.0),	// u1 wire 92
  CH_OHIPS_POSITION_U1+93.*25.4/sqrt(32.0),	// u1 wire 93
  CH_OHIPS_POSITION_U1+94.*25.4/sqrt(32.0),	// u1 wire 94
  CH_OHIPS_POSITION_U1+95.*25.4/sqrt(32.0),	// u1 wire 95
  CH_OHIPS_POSITION_U1+96.*25.4/sqrt(32.0),	// u1 wire 96
  CH_OHIPS_POSITION_U1+97.*25.4/sqrt(32.0),	// u1 wire 97
  CH_OHIPS_POSITION_U1+98.*25.4/sqrt(32.0),	// u1 wire 98
  CH_OHIPS_POSITION_U1+99.*25.4/sqrt(32.0),	// u1 wire 99
  CH_OHIPS_POSITION_U1+100.*25.4/sqrt(32.0),	// u1 wire 100
  CH_OHIPS_POSITION_U1+101.*25.4/sqrt(32.0),	// u1 wire 101
  CH_OHIPS_POSITION_U1+102.*25.4/sqrt(32.0),	// u1 wire 102
  CH_OHIPS_POSITION_U1+103.*25.4/sqrt(32.0),	// u1 wire 103
  CH_OHIPS_POSITION_U1+104.*25.4/sqrt(32.0),	// u1 wire 104
  CH_OHIPS_POSITION_U1+105.*25.4/sqrt(32.0),	// u1 wire 105
  CH_OHIPS_POSITION_U1+106.*25.4/sqrt(32.0),	// u1 wire 106
  CH_OHIPS_POSITION_U1+107.*25.4/sqrt(32.0),	// u1 wire 107
  CH_OHIPS_POSITION_U1+108.*25.4/sqrt(32.0),	// u1 wire 108
  CH_OHIPS_POSITION_U1+109.*25.4/sqrt(32.0),	// u1 wire 109
  CH_OHIPS_POSITION_U1+110.*25.4/sqrt(32.0),	// u1 wire 110
  CH_OHIPS_POSITION_U1+111.*25.4/sqrt(32.0),	// u1 wire 111
  CH_OHIPS_POSITION_U1+112.*25.4/sqrt(32.0),	// u1 wire 112
  CH_OHIPS_POSITION_U1+113.*25.4/sqrt(32.0),	// u1 wire 113
  CH_OHIPS_POSITION_U1+114.*25.4/sqrt(32.0),	// u1 wire 114
  CH_OHIPS_POSITION_U1+115.*25.4/sqrt(32.0),	// u1 wire 115
  CH_OHIPS_POSITION_U1+116.*25.4/sqrt(32.0),	// u1 wire 116
  CH_OHIPS_POSITION_U1+117.*25.4/sqrt(32.0),	// u1 wire 117
  CH_OHIPS_POSITION_U1+118.*25.4/sqrt(32.0),	// u1 wire 118
  CH_OHIPS_POSITION_U1+119.*25.4/sqrt(32.0),	// u1 wire 119
  CH_OHIPS_POSITION_U1+120.*25.4/sqrt(32.0),	// u1 wire 120
  CH_OHIPS_POSITION_U1+121.*25.4/sqrt(32.0),	// u1 wire 121
  CH_OHIPS_POSITION_U1+122.*25.4/sqrt(32.0),	// u1 wire 122
  CH_OHIPS_POSITION_U1+123.*25.4/sqrt(32.0),	// u1 wire 123
  CH_OHIPS_POSITION_U1+124.*25.4/sqrt(32.0),	// u1 wire 124
  CH_OHIPS_POSITION_U1+125.*25.4/sqrt(32.0),	// u1 wire 125
  CH_OHIPS_POSITION_U1+126.*25.4/sqrt(32.0),	// u1 wire 126
  CH_OHIPS_POSITION_U1+127.*25.4/sqrt(32.0),	// u1 wire 127
  CH_OHIPS_POSITION_U1+128.*25.4/sqrt(32.0),	// u1 wire 128
  CH_OHIPS_POSITION_U1+129.*25.4/sqrt(32.0),	// u1 wire 129
  CH_OHIPS_POSITION_U1+130.*25.4/sqrt(32.0),	// u1 wire 130
  CH_OHIPS_POSITION_U1+131.*25.4/sqrt(32.0),	// u1 wire 131
  CH_OHIPS_POSITION_U1+132.*25.4/sqrt(32.0),	// u1 wire 132
  CH_OHIPS_POSITION_U1+133.*25.4/sqrt(32.0),	// u1 wire 133
  CH_OHIPS_POSITION_U1+134.*25.4/sqrt(32.0),	// u1 wire 134
  CH_OHIPS_POSITION_U1+135.*25.4/sqrt(32.0),	// u1 wire 135
  CH_OHIPS_POSITION_U1+136.*25.4/sqrt(32.0),	// u1 wire 136
  CH_OHIPS_POSITION_U1+137.*25.4/sqrt(32.0),	// u1 wire 137
  CH_OHIPS_POSITION_U1+138.*25.4/sqrt(32.0),	// u1 wire 138
  CH_OHIPS_POSITION_U1+139.*25.4/sqrt(32.0),	// u1 wire 139
  CH_OHIPS_POSITION_U1+140.*25.4/sqrt(32.0),	// u1 wire 140
  CH_OHIPS_POSITION_U1+141.*25.4/sqrt(32.0),	// u1 wire 141
  CH_OHIPS_POSITION_U1+142.*25.4/sqrt(32.0),	// u1 wire 142
  CH_OHIPS_POSITION_U1+143.*25.4/sqrt(32.0),	// u1 wire 143
  CH_OHIPS_POSITION_U1+144.*25.4/sqrt(32.0),	// u1 wire 144
  CH_OHIPS_POSITION_U1+145.*25.4/sqrt(32.0),	// u1 wire 145
  CH_OHIPS_POSITION_U1+146.*25.4/sqrt(32.0),	// u1 wire 146
  CH_OHIPS_POSITION_U1+147.*25.4/sqrt(32.0),	// u1 wire 147
  CH_OHIPS_POSITION_U1+148.*25.4/sqrt(32.0),	// u1 wire 148
  CH_OHIPS_POSITION_U1+149.*25.4/sqrt(32.0),	// u1 wire 149
  CH_OHIPS_POSITION_U1+150.*25.4/sqrt(32.0),	// u1 wire 150
  CH_OHIPS_POSITION_U1+151.*25.4/sqrt(32.0),	// u1 wire 151
  CH_OHIPS_POSITION_U1+152.*25.4/sqrt(32.0),	// u1 wire 152
  CH_OHIPS_POSITION_U1+153.*25.4/sqrt(32.0),	// u1 wire 153
  CH_OHIPS_POSITION_U1+154.*25.4/sqrt(32.0),	// u1 wire 154
  CH_OHIPS_POSITION_U1+155.*25.4/sqrt(32.0),	// u1 wire 155
  CH_OHIPS_POSITION_U1+156.*25.4/sqrt(32.0),	// u1 wire 156
  CH_OHIPS_POSITION_U1+157.*25.4/sqrt(32.0),	// u1 wire 157
  CH_OHIPS_POSITION_U1+158.*25.4/sqrt(32.0),	// u1 wire 158
  CH_OHIPS_POSITION_U1+159.*25.4/sqrt(32.0),	// u1 wire 159
  CH_OHIPS_POSITION_U1+160.*25.4/sqrt(32.0),	// u1 wire 160
  CH_OHIPS_POSITION_U1+161.*25.4/sqrt(32.0),	// u1 wire 161
  CH_OHIPS_POSITION_U1+162.*25.4/sqrt(32.0),	// u1 wire 162
  CH_OHIPS_POSITION_U1+163.*25.4/sqrt(32.0),	// u1 wire 163
  CH_OHIPS_POSITION_U1+164.*25.4/sqrt(32.0),	// u1 wire 164
  CH_OHIPS_POSITION_U1+165.*25.4/sqrt(32.0),	// u1 wire 165
  CH_OHIPS_POSITION_U1+166.*25.4/sqrt(32.0),	// u1 wire 166
  CH_OHIPS_POSITION_U1+167.*25.4/sqrt(32.0),	// u1 wire 167
  CH_OHIPS_POSITION_U1+168.*25.4/sqrt(32.0),	// u1 wire 168
  CH_OHIPS_POSITION_U1+169.*25.4/sqrt(32.0),	// u1 wire 169
  CH_OHIPS_POSITION_U1+170.*25.4/sqrt(32.0),	// u1 wire 170
  CH_OHIPS_POSITION_U1+171.*25.4/sqrt(32.0),	// u1 wire 171
  CH_OHIPS_POSITION_U1+172.*25.4/sqrt(32.0),	// u1 wire 172
  CH_OHIPS_POSITION_U1+173.*25.4/sqrt(32.0),	// u1 wire 173
  CH_OHIPS_POSITION_U1+174.*25.4/sqrt(32.0),	// u1 wire 174
  CH_OHIPS_POSITION_U1+175.*25.4/sqrt(32.0),	// u1 wire 175
  CH_OHIPS_POSITION_U1+176.*25.4/sqrt(32.0),	// u1 wire 176
  CH_OHIPS_POSITION_U1+177.*25.4/sqrt(32.0),	// u1 wire 177
  CH_OHIPS_POSITION_U1+178.*25.4/sqrt(32.0),	// u1 wire 178
  CH_OHIPS_POSITION_U1+179.*25.4/sqrt(32.0),	// u1 wire 179
  CH_OHIPS_POSITION_U1+180.*25.4/sqrt(32.0),	// u1 wire 180
  CH_OHIPS_POSITION_U1+181.*25.4/sqrt(32.0),	// u1 wire 181
  CH_OHIPS_POSITION_U1+182.*25.4/sqrt(32.0),	// u1 wire 182
  CH_OHIPS_POSITION_U1+183.*25.4/sqrt(32.0),	// u1 wire 183
  CH_OHIPS_POSITION_U1+184.*25.4/sqrt(32.0),	// u1 wire 184
  CH_OHIPS_POSITION_U1+185.*25.4/sqrt(32.0),	// u1 wire 185
  CH_OHIPS_POSITION_U1+186.*25.4/sqrt(32.0),	// u1 wire 186
  CH_OHIPS_POSITION_U1+187.*25.4/sqrt(32.0),	// u1 wire 187
  CH_OHIPS_POSITION_U1+188.*25.4/sqrt(32.0),	// u1 wire 188
  CH_OHIPS_POSITION_U1+189.*25.4/sqrt(32.0),	// u1 wire 189
  CH_OHIPS_POSITION_U1+190.*25.4/sqrt(32.0),	// u1 wire 190
  CH_OHIPS_POSITION_U1+191.*25.4/sqrt(32.0),	// u1 wire 191
  CH_OHIPS_POSITION_U1+192.*25.4/sqrt(32.0),	// u1 wire 192
  CH_OHIPS_POSITION_U1+193.*25.4/sqrt(32.0),	// u1 wire 193
  CH_OHIPS_POSITION_U1+194.*25.4/sqrt(32.0),	// u1 wire 194
  CH_OHIPS_POSITION_U1+195.*25.4/sqrt(32.0),	// u1 wire 195
  CH_OHIPS_POSITION_U1+196.*25.4/sqrt(32.0),	// u1 wire 196
  CH_OHIPS_POSITION_U1+197.*25.4/sqrt(32.0),	// u1 wire 197
  CH_OHIPS_POSITION_U1+198.*25.4/sqrt(32.0),	// u1 wire 198
  CH_OHIPS_POSITION_U1+199.*25.4/sqrt(32.0),	// u1 wire 199
  CH_OHIPS_POSITION_U1+200.*25.4/sqrt(32.0),	// u1 wire 200
  CH_OHIPS_POSITION_U1+201.*25.4/sqrt(32.0),	// u1 wire 201
  CH_OHIPS_POSITION_U1+202.*25.4/sqrt(32.0),	// u1 wire 202
  CH_OHIPS_POSITION_U1+203.*25.4/sqrt(32.0),	// u1 wire 203
  CH_OHIPS_POSITION_U1+204.*25.4/sqrt(32.0),	// u1 wire 204
  CH_OHIPS_POSITION_U1+205.*25.4/sqrt(32.0),	// u1 wire 205
  CH_OHIPS_POSITION_U1+206.*25.4/sqrt(32.0),	// u1 wire 206
  CH_OHIPS_POSITION_U1+207.*25.4/sqrt(32.0),	// u1 wire 207
  CH_OHIPS_POSITION_U1+208.*25.4/sqrt(32.0),	// u1 wire 208
  CH_OHIPS_POSITION_U1+209.*25.4/sqrt(32.0),	// u1 wire 209
  CH_OHIPS_POSITION_U1+210.*25.4/sqrt(32.0),	// u1 wire 210
  CH_OHIPS_POSITION_U1+211.*25.4/sqrt(32.0),	// u1 wire 211
  CH_OHIPS_POSITION_U1+212.*25.4/sqrt(32.0),	// u1 wire 212
  CH_OHIPS_POSITION_U1+213.*25.4/sqrt(32.0),	// u1 wire 213
  CH_OHIPS_POSITION_U1+214.*25.4/sqrt(32.0),	// u1 wire 214
  CH_OHIPS_POSITION_U1+215.*25.4/sqrt(32.0),	// u1 wire 215
  CH_OHIPS_POSITION_U1+216.*25.4/sqrt(32.0),	// u1 wire 216
  CH_OHIPS_POSITION_U1+217.*25.4/sqrt(32.0),	// u1 wire 217
  CH_OHIPS_POSITION_U1+218.*25.4/sqrt(32.0),	// u1 wire 218
  CH_OHIPS_POSITION_U1+219.*25.4/sqrt(32.0),	// u1 wire 219
  CH_OHIPS_POSITION_U1+220.*25.4/sqrt(32.0),	// u1 wire 220
  CH_OHIPS_POSITION_U1+221.*25.4/sqrt(32.0),	// u1 wire 221
  CH_OHIPS_POSITION_U1+222.*25.4/sqrt(32.0),	// u1 wire 222
  CH_OHIPS_POSITION_U1+223.*25.4/sqrt(32.0),	// u1 wire 223
  CH_OHIPS_POSITION_U1+224.*25.4/sqrt(32.0),	// u1 wire 224
  CH_OHIPS_POSITION_U1+225.*25.4/sqrt(32.0),	// u1 wire 225
  CH_OHIPS_POSITION_U1+226.*25.4/sqrt(32.0),	// u1 wire 226
  CH_OHIPS_POSITION_U1+227.*25.4/sqrt(32.0),	// u1 wire 227
  CH_OHIPS_POSITION_U1+228.*25.4/sqrt(32.0),	// u1 wire 228
  CH_OHIPS_POSITION_U1+229.*25.4/sqrt(32.0),	// u1 wire 229
  CH_OHIPS_POSITION_U1+230.*25.4/sqrt(32.0),	// u1 wire 230
  CH_OHIPS_POSITION_U1+231.*25.4/sqrt(32.0),	// u1 wire 231
  CH_OHIPS_POSITION_U1+232.*25.4/sqrt(32.0),	// u1 wire 232
  CH_OHIPS_POSITION_U1+233.*25.4/sqrt(32.0),	// u1 wire 233
  CH_OHIPS_POSITION_U1+234.*25.4/sqrt(32.0),	// u1 wire 234
  CH_OHIPS_POSITION_U1+235.*25.4/sqrt(32.0),	// u1 wire 235
  CH_OHIPS_POSITION_U1+236.*25.4/sqrt(32.0),	// u1 wire 236
  CH_OHIPS_POSITION_U1+237.*25.4/sqrt(32.0),	// u1 wire 237
  CH_OHIPS_POSITION_U1+238.*25.4/sqrt(32.0),	// u1 wire 238
  CH_OHIPS_POSITION_U1+239.*25.4/sqrt(32.0),	// u1 wire 239
  CH_OHIPS_POSITION_U1+240.*25.4/sqrt(32.0),	// u1 wire 240
  CH_OHIPS_POSITION_U1+241.*25.4/sqrt(32.0),	// u1 wire 241
  CH_OHIPS_POSITION_U1+242.*25.4/sqrt(32.0),	// u1 wire 242
  CH_OHIPS_POSITION_U1+243.*25.4/sqrt(32.0),	// u1 wire 243
  CH_OHIPS_POSITION_U1+244.*25.4/sqrt(32.0),	// u1 wire 244
  CH_OHIPS_POSITION_U1+245.*25.4/sqrt(32.0),	// u1 wire 245
  CH_OHIPS_POSITION_U1+246.*25.4/sqrt(32.0),	// u1 wire 246
  CH_OHIPS_POSITION_U1+247.*25.4/sqrt(32.0),	// u1 wire 247
  CH_OHIPS_POSITION_U1+248.*25.4/sqrt(32.0),	// u1 wire 248
  CH_OHIPS_POSITION_U1+249.*25.4/sqrt(32.0),	// u1 wire 249
  CH_OHIPS_POSITION_U1+250.*25.4/sqrt(32.0),	// u1 wire 250
  CH_OHIPS_POSITION_U1+251.*25.4/sqrt(32.0),	// u1 wire 251
  CH_OHIPS_POSITION_U1+252.*25.4/sqrt(32.0),	// u1 wire 252
  CH_OHIPS_POSITION_U1+253.*25.4/sqrt(32.0),	// u1 wire 253
  CH_OHIPS_POSITION_U1+254.*25.4/sqrt(32.0),	// u1 wire 254
  CH_OHIPS_POSITION_U1+255.*25.4/sqrt(32.0),	// u1 wire 255
  CH_OHIPS_POSITION_U1+256.*25.4/sqrt(32.0)	// u1 wire 256
};


static float
position_ov1[CH_OHIPS_MAX_V1] = {
  CH_OHIPS_POSITION_V1+1.*25.4/sqrt(32.0),	// v1 wire 1
  CH_OHIPS_POSITION_V1+2.*25.4/sqrt(32.0),	// v1 wire 2
  CH_OHIPS_POSITION_V1+3.*25.4/sqrt(32.0),	// v1 wire 3
  CH_OHIPS_POSITION_V1+4.*25.4/sqrt(32.0),	// v1 wire 4
  CH_OHIPS_POSITION_V1+5.*25.4/sqrt(32.0),	// v1 wire 5
  CH_OHIPS_POSITION_V1+6.*25.4/sqrt(32.0),	// v1 wire 6
  CH_OHIPS_POSITION_V1+7.*25.4/sqrt(32.0),	// v1 wire 7
  CH_OHIPS_POSITION_V1+8.*25.4/sqrt(32.0),	// v1 wire 8
  CH_OHIPS_POSITION_V1+9.*25.4/sqrt(32.0),	// v1 wire 9
  CH_OHIPS_POSITION_V1+10.*25.4/sqrt(32.0),	// v1 wire 10
  CH_OHIPS_POSITION_V1+11.*25.4/sqrt(32.0),	// v1 wire 11
  CH_OHIPS_POSITION_V1+12.*25.4/sqrt(32.0),	// v1 wire 12
  CH_OHIPS_POSITION_V1+13.*25.4/sqrt(32.0),	// v1 wire 13
  CH_OHIPS_POSITION_V1+14.*25.4/sqrt(32.0),	// v1 wire 14
  CH_OHIPS_POSITION_V1+15.*25.4/sqrt(32.0),	// v1 wire 15
  CH_OHIPS_POSITION_V1+16.*25.4/sqrt(32.0),	// v1 wire 16
  CH_OHIPS_POSITION_V1+17.*25.4/sqrt(32.0),	// v1 wire 17
  CH_OHIPS_POSITION_V1+18.*25.4/sqrt(32.0),	// v1 wire 18
  CH_OHIPS_POSITION_V1+19.*25.4/sqrt(32.0),	// v1 wire 19
  CH_OHIPS_POSITION_V1+20.*25.4/sqrt(32.0),	// v1 wire 20
  CH_OHIPS_POSITION_V1+21.*25.4/sqrt(32.0),	// v1 wire 21
  CH_OHIPS_POSITION_V1+22.*25.4/sqrt(32.0),	// v1 wire 22
  CH_OHIPS_POSITION_V1+23.*25.4/sqrt(32.0),	// v1 wire 23
  CH_OHIPS_POSITION_V1+24.*25.4/sqrt(32.0),	// v1 wire 24
  CH_OHIPS_POSITION_V1+25.*25.4/sqrt(32.0),	// v1 wire 25
  CH_OHIPS_POSITION_V1+26.*25.4/sqrt(32.0),	// v1 wire 26
  CH_OHIPS_POSITION_V1+27.*25.4/sqrt(32.0),	// v1 wire 27
  CH_OHIPS_POSITION_V1+28.*25.4/sqrt(32.0),	// v1 wire 28
  CH_OHIPS_POSITION_V1+29.*25.4/sqrt(32.0),	// v1 wire 29
  CH_OHIPS_POSITION_V1+30.*25.4/sqrt(32.0),	// v1 wire 30
  CH_OHIPS_POSITION_V1+31.*25.4/sqrt(32.0),	// v1 wire 31
  CH_OHIPS_POSITION_V1+32.*25.4/sqrt(32.0),	// v1 wire 32
  CH_OHIPS_POSITION_V1+33.*25.4/sqrt(32.0),	// v1 wire 33
  CH_OHIPS_POSITION_V1+34.*25.4/sqrt(32.0),	// v1 wire 34
  CH_OHIPS_POSITION_V1+35.*25.4/sqrt(32.0),	// v1 wire 35
  CH_OHIPS_POSITION_V1+36.*25.4/sqrt(32.0),	// v1 wire 36
  CH_OHIPS_POSITION_V1+37.*25.4/sqrt(32.0),	// v1 wire 37
  CH_OHIPS_POSITION_V1+38.*25.4/sqrt(32.0),	// v1 wire 38
  CH_OHIPS_POSITION_V1+39.*25.4/sqrt(32.0),	// v1 wire 39
  CH_OHIPS_POSITION_V1+40.*25.4/sqrt(32.0),	// v1 wire 40
  CH_OHIPS_POSITION_V1+41.*25.4/sqrt(32.0),	// v1 wire 41
  CH_OHIPS_POSITION_V1+42.*25.4/sqrt(32.0),	// v1 wire 42
  CH_OHIPS_POSITION_V1+43.*25.4/sqrt(32.0),	// v1 wire 43
  CH_OHIPS_POSITION_V1+44.*25.4/sqrt(32.0),	// v1 wire 44
  CH_OHIPS_POSITION_V1+45.*25.4/sqrt(32.0),	// v1 wire 45
  CH_OHIPS_POSITION_V1+46.*25.4/sqrt(32.0),	// v1 wire 46
  CH_OHIPS_POSITION_V1+47.*25.4/sqrt(32.0),	// v1 wire 47
  CH_OHIPS_POSITION_V1+48.*25.4/sqrt(32.0),	// v1 wire 48
  CH_OHIPS_POSITION_V1+49.*25.4/sqrt(32.0),	// v1 wire 49
  CH_OHIPS_POSITION_V1+50.*25.4/sqrt(32.0),	// v1 wire 50
  CH_OHIPS_POSITION_V1+51.*25.4/sqrt(32.0),	// v1 wire 51
  CH_OHIPS_POSITION_V1+52.*25.4/sqrt(32.0),	// v1 wire 52
  CH_OHIPS_POSITION_V1+53.*25.4/sqrt(32.0),	// v1 wire 53
  CH_OHIPS_POSITION_V1+54.*25.4/sqrt(32.0),	// v1 wire 54
  CH_OHIPS_POSITION_V1+55.*25.4/sqrt(32.0),	// v1 wire 55
  CH_OHIPS_POSITION_V1+56.*25.4/sqrt(32.0),	// v1 wire 56
  CH_OHIPS_POSITION_V1+57.*25.4/sqrt(32.0),	// v1 wire 57
  CH_OHIPS_POSITION_V1+58.*25.4/sqrt(32.0),	// v1 wire 58
  CH_OHIPS_POSITION_V1+59.*25.4/sqrt(32.0),	// v1 wire 59
  CH_OHIPS_POSITION_V1+60.*25.4/sqrt(32.0),	// v1 wire 60
  CH_OHIPS_POSITION_V1+61.*25.4/sqrt(32.0),	// v1 wire 61
  CH_OHIPS_POSITION_V1+62.*25.4/sqrt(32.0),	// v1 wire 62
  CH_OHIPS_POSITION_V1+63.*25.4/sqrt(32.0),	// v1 wire 63
  CH_OHIPS_POSITION_V1+64.*25.4/sqrt(32.0),	// v1 wire 64
  CH_OHIPS_POSITION_V1+65.*25.4/sqrt(32.0),	// v1 wire 65
  CH_OHIPS_POSITION_V1+66.*25.4/sqrt(32.0),	// v1 wire 66
  CH_OHIPS_POSITION_V1+67.*25.4/sqrt(32.0),	// v1 wire 67
  CH_OHIPS_POSITION_V1+68.*25.4/sqrt(32.0),	// v1 wire 68
  CH_OHIPS_POSITION_V1+69.*25.4/sqrt(32.0),	// v1 wire 69
  CH_OHIPS_POSITION_V1+70.*25.4/sqrt(32.0),	// v1 wire 70
  CH_OHIPS_POSITION_V1+71.*25.4/sqrt(32.0),	// v1 wire 71
  CH_OHIPS_POSITION_V1+72.*25.4/sqrt(32.0),	// v1 wire 72
  CH_OHIPS_POSITION_V1+73.*25.4/sqrt(32.0),	// v1 wire 73
  CH_OHIPS_POSITION_V1+74.*25.4/sqrt(32.0),	// v1 wire 74
  CH_OHIPS_POSITION_V1+75.*25.4/sqrt(32.0),	// v1 wire 75
  CH_OHIPS_POSITION_V1+76.*25.4/sqrt(32.0),	// v1 wire 76
  CH_OHIPS_POSITION_V1+77.*25.4/sqrt(32.0),	// v1 wire 77
  CH_OHIPS_POSITION_V1+78.*25.4/sqrt(32.0),	// v1 wire 78
  CH_OHIPS_POSITION_V1+79.*25.4/sqrt(32.0),	// v1 wire 79
  CH_OHIPS_POSITION_V1+80.*25.4/sqrt(32.0),	// v1 wire 80
  CH_OHIPS_POSITION_V1+81.*25.4/sqrt(32.0),	// v1 wire 81
  CH_OHIPS_POSITION_V1+82.*25.4/sqrt(32.0),	// v1 wire 82
  CH_OHIPS_POSITION_V1+83.*25.4/sqrt(32.0),	// v1 wire 83
  CH_OHIPS_POSITION_V1+84.*25.4/sqrt(32.0),	// v1 wire 84
  CH_OHIPS_POSITION_V1+85.*25.4/sqrt(32.0),	// v1 wire 85
  CH_OHIPS_POSITION_V1+86.*25.4/sqrt(32.0),	// v1 wire 86
  CH_OHIPS_POSITION_V1+87.*25.4/sqrt(32.0),	// v1 wire 87
  CH_OHIPS_POSITION_V1+88.*25.4/sqrt(32.0),	// v1 wire 88
  CH_OHIPS_POSITION_V1+89.*25.4/sqrt(32.0),	// v1 wire 89
  CH_OHIPS_POSITION_V1+90.*25.4/sqrt(32.0),	// v1 wire 90
  CH_OHIPS_POSITION_V1+91.*25.4/sqrt(32.0),	// v1 wire 91
  CH_OHIPS_POSITION_V1+92.*25.4/sqrt(32.0),	// v1 wire 92
  CH_OHIPS_POSITION_V1+93.*25.4/sqrt(32.0),	// v1 wire 93
  CH_OHIPS_POSITION_V1+94.*25.4/sqrt(32.0),	// v1 wire 94
  CH_OHIPS_POSITION_V1+95.*25.4/sqrt(32.0),	// v1 wire 95
  CH_OHIPS_POSITION_V1+96.*25.4/sqrt(32.0),	// v1 wire 96
  CH_OHIPS_POSITION_V1+97.*25.4/sqrt(32.0),	// v1 wire 97
  CH_OHIPS_POSITION_V1+98.*25.4/sqrt(32.0),	// v1 wire 98
  CH_OHIPS_POSITION_V1+99.*25.4/sqrt(32.0),	// v1 wire 99
  CH_OHIPS_POSITION_V1+100.*25.4/sqrt(32.0),	// v1 wire 100
  CH_OHIPS_POSITION_V1+101.*25.4/sqrt(32.0),	// v1 wire 101
  CH_OHIPS_POSITION_V1+102.*25.4/sqrt(32.0),	// v1 wire 102
  CH_OHIPS_POSITION_V1+103.*25.4/sqrt(32.0),	// v1 wire 103
  CH_OHIPS_POSITION_V1+104.*25.4/sqrt(32.0),	// v1 wire 104
  CH_OHIPS_POSITION_V1+105.*25.4/sqrt(32.0),	// v1 wire 105
  CH_OHIPS_POSITION_V1+106.*25.4/sqrt(32.0),	// v1 wire 106
  CH_OHIPS_POSITION_V1+107.*25.4/sqrt(32.0),	// v1 wire 107
  CH_OHIPS_POSITION_V1+108.*25.4/sqrt(32.0),	// v1 wire 108
  CH_OHIPS_POSITION_V1+109.*25.4/sqrt(32.0),	// v1 wire 109
  CH_OHIPS_POSITION_V1+110.*25.4/sqrt(32.0),	// v1 wire 110
  CH_OHIPS_POSITION_V1+111.*25.4/sqrt(32.0),	// v1 wire 111
  CH_OHIPS_POSITION_V1+112.*25.4/sqrt(32.0),	// v1 wire 112
  CH_OHIPS_POSITION_V1+113.*25.4/sqrt(32.0),	// v1 wire 113
  CH_OHIPS_POSITION_V1+114.*25.4/sqrt(32.0),	// v1 wire 114
  CH_OHIPS_POSITION_V1+115.*25.4/sqrt(32.0),	// v1 wire 115
  CH_OHIPS_POSITION_V1+116.*25.4/sqrt(32.0),	// v1 wire 116
  CH_OHIPS_POSITION_V1+117.*25.4/sqrt(32.0),	// v1 wire 117
  CH_OHIPS_POSITION_V1+118.*25.4/sqrt(32.0),	// v1 wire 118
  CH_OHIPS_POSITION_V1+119.*25.4/sqrt(32.0),	// v1 wire 119
  CH_OHIPS_POSITION_V1+120.*25.4/sqrt(32.0),	// v1 wire 120
  CH_OHIPS_POSITION_V1+121.*25.4/sqrt(32.0),	// v1 wire 121
  CH_OHIPS_POSITION_V1+122.*25.4/sqrt(32.0),	// v1 wire 122
  CH_OHIPS_POSITION_V1+123.*25.4/sqrt(32.0),	// v1 wire 123
  CH_OHIPS_POSITION_V1+124.*25.4/sqrt(32.0),	// v1 wire 124
  CH_OHIPS_POSITION_V1+125.*25.4/sqrt(32.0),	// v1 wire 125
  CH_OHIPS_POSITION_V1+126.*25.4/sqrt(32.0),	// v1 wire 126
  CH_OHIPS_POSITION_V1+127.*25.4/sqrt(32.0),	// v1 wire 127
  CH_OHIPS_POSITION_V1+128.*25.4/sqrt(32.0),	// v1 wire 128
  CH_OHIPS_POSITION_V1+129.*25.4/sqrt(32.0),	// v1 wire 129
  CH_OHIPS_POSITION_V1+130.*25.4/sqrt(32.0),	// v1 wire 130
  CH_OHIPS_POSITION_V1+131.*25.4/sqrt(32.0),	// v1 wire 131
  CH_OHIPS_POSITION_V1+132.*25.4/sqrt(32.0),	// v1 wire 132
  CH_OHIPS_POSITION_V1+133.*25.4/sqrt(32.0),	// v1 wire 133
  CH_OHIPS_POSITION_V1+134.*25.4/sqrt(32.0),	// v1 wire 134
  CH_OHIPS_POSITION_V1+135.*25.4/sqrt(32.0),	// v1 wire 135
  CH_OHIPS_POSITION_V1+136.*25.4/sqrt(32.0),	// v1 wire 136
  CH_OHIPS_POSITION_V1+137.*25.4/sqrt(32.0),	// v1 wire 137
  CH_OHIPS_POSITION_V1+138.*25.4/sqrt(32.0),	// v1 wire 138
  CH_OHIPS_POSITION_V1+139.*25.4/sqrt(32.0),	// v1 wire 139
  CH_OHIPS_POSITION_V1+140.*25.4/sqrt(32.0),	// v1 wire 140
  CH_OHIPS_POSITION_V1+141.*25.4/sqrt(32.0),	// v1 wire 141
  CH_OHIPS_POSITION_V1+142.*25.4/sqrt(32.0),	// v1 wire 142
  CH_OHIPS_POSITION_V1+143.*25.4/sqrt(32.0),	// v1 wire 143
  CH_OHIPS_POSITION_V1+144.*25.4/sqrt(32.0),	// v1 wire 144
  CH_OHIPS_POSITION_V1+145.*25.4/sqrt(32.0),	// v1 wire 145
  CH_OHIPS_POSITION_V1+146.*25.4/sqrt(32.0),	// v1 wire 146
  CH_OHIPS_POSITION_V1+147.*25.4/sqrt(32.0),	// v1 wire 147
  CH_OHIPS_POSITION_V1+148.*25.4/sqrt(32.0),	// v1 wire 148
  CH_OHIPS_POSITION_V1+149.*25.4/sqrt(32.0),	// v1 wire 149
  CH_OHIPS_POSITION_V1+150.*25.4/sqrt(32.0),	// v1 wire 150
  CH_OHIPS_POSITION_V1+151.*25.4/sqrt(32.0),	// v1 wire 151
  CH_OHIPS_POSITION_V1+152.*25.4/sqrt(32.0),	// v1 wire 152
  CH_OHIPS_POSITION_V1+153.*25.4/sqrt(32.0),	// v1 wire 153
  CH_OHIPS_POSITION_V1+154.*25.4/sqrt(32.0),	// v1 wire 154
  CH_OHIPS_POSITION_V1+155.*25.4/sqrt(32.0),	// v1 wire 155
  CH_OHIPS_POSITION_V1+156.*25.4/sqrt(32.0),	// v1 wire 156
  CH_OHIPS_POSITION_V1+157.*25.4/sqrt(32.0),	// v1 wire 157
  CH_OHIPS_POSITION_V1+158.*25.4/sqrt(32.0),	// v1 wire 158
  CH_OHIPS_POSITION_V1+159.*25.4/sqrt(32.0),	// v1 wire 159
  CH_OHIPS_POSITION_V1+160.*25.4/sqrt(32.0),	// v1 wire 160
  CH_OHIPS_POSITION_V1+161.*25.4/sqrt(32.0),	// v1 wire 161
  CH_OHIPS_POSITION_V1+162.*25.4/sqrt(32.0),	// v1 wire 162
  CH_OHIPS_POSITION_V1+163.*25.4/sqrt(32.0),	// v1 wire 163
  CH_OHIPS_POSITION_V1+164.*25.4/sqrt(32.0),	// v1 wire 164
  CH_OHIPS_POSITION_V1+165.*25.4/sqrt(32.0),	// v1 wire 165
  CH_OHIPS_POSITION_V1+166.*25.4/sqrt(32.0),	// v1 wire 166
  CH_OHIPS_POSITION_V1+167.*25.4/sqrt(32.0),	// v1 wire 167
  CH_OHIPS_POSITION_V1+168.*25.4/sqrt(32.0),	// v1 wire 168
  CH_OHIPS_POSITION_V1+169.*25.4/sqrt(32.0),	// v1 wire 169
  CH_OHIPS_POSITION_V1+170.*25.4/sqrt(32.0),	// v1 wire 170
  CH_OHIPS_POSITION_V1+171.*25.4/sqrt(32.0),	// v1 wire 171
  CH_OHIPS_POSITION_V1+172.*25.4/sqrt(32.0),	// v1 wire 172
  CH_OHIPS_POSITION_V1+173.*25.4/sqrt(32.0),	// v1 wire 173
  CH_OHIPS_POSITION_V1+174.*25.4/sqrt(32.0),	// v1 wire 174
  CH_OHIPS_POSITION_V1+175.*25.4/sqrt(32.0),	// v1 wire 175
  CH_OHIPS_POSITION_V1+176.*25.4/sqrt(32.0),	// v1 wire 176
  CH_OHIPS_POSITION_V1+177.*25.4/sqrt(32.0),	// v1 wire 177
  CH_OHIPS_POSITION_V1+178.*25.4/sqrt(32.0),	// v1 wire 178
  CH_OHIPS_POSITION_V1+179.*25.4/sqrt(32.0),	// v1 wire 179
  CH_OHIPS_POSITION_V1+180.*25.4/sqrt(32.0),	// v1 wire 180
  CH_OHIPS_POSITION_V1+181.*25.4/sqrt(32.0),	// v1 wire 181
  CH_OHIPS_POSITION_V1+182.*25.4/sqrt(32.0),	// v1 wire 182
  CH_OHIPS_POSITION_V1+183.*25.4/sqrt(32.0),	// v1 wire 183
  CH_OHIPS_POSITION_V1+184.*25.4/sqrt(32.0),	// v1 wire 184
  CH_OHIPS_POSITION_V1+185.*25.4/sqrt(32.0),	// v1 wire 185
  CH_OHIPS_POSITION_V1+186.*25.4/sqrt(32.0),	// v1 wire 186
  CH_OHIPS_POSITION_V1+187.*25.4/sqrt(32.0),	// v1 wire 187
  CH_OHIPS_POSITION_V1+188.*25.4/sqrt(32.0),	// v1 wire 188
  CH_OHIPS_POSITION_V1+189.*25.4/sqrt(32.0),	// v1 wire 189
  CH_OHIPS_POSITION_V1+190.*25.4/sqrt(32.0),	// v1 wire 190
  CH_OHIPS_POSITION_V1+191.*25.4/sqrt(32.0),	// v1 wire 191
  CH_OHIPS_POSITION_V1+192.*25.4/sqrt(32.0),	// v1 wire 192
  CH_OHIPS_POSITION_V1+193.*25.4/sqrt(32.0),	// v1 wire 193
  CH_OHIPS_POSITION_V1+194.*25.4/sqrt(32.0),	// v1 wire 194
  CH_OHIPS_POSITION_V1+195.*25.4/sqrt(32.0),	// v1 wire 195
  CH_OHIPS_POSITION_V1+196.*25.4/sqrt(32.0),	// v1 wire 196
  CH_OHIPS_POSITION_V1+197.*25.4/sqrt(32.0),	// v1 wire 197
  CH_OHIPS_POSITION_V1+198.*25.4/sqrt(32.0),	// v1 wire 198
  CH_OHIPS_POSITION_V1+199.*25.4/sqrt(32.0),	// v1 wire 199
  CH_OHIPS_POSITION_V1+200.*25.4/sqrt(32.0),	// v1 wire 200
  CH_OHIPS_POSITION_V1+201.*25.4/sqrt(32.0),	// v1 wire 201
  CH_OHIPS_POSITION_V1+202.*25.4/sqrt(32.0),	// v1 wire 202
  CH_OHIPS_POSITION_V1+203.*25.4/sqrt(32.0),	// v1 wire 203
  CH_OHIPS_POSITION_V1+204.*25.4/sqrt(32.0),	// v1 wire 204
  CH_OHIPS_POSITION_V1+205.*25.4/sqrt(32.0),	// v1 wire 205
  CH_OHIPS_POSITION_V1+206.*25.4/sqrt(32.0),	// v1 wire 206
  CH_OHIPS_POSITION_V1+207.*25.4/sqrt(32.0),	// v1 wire 207
  CH_OHIPS_POSITION_V1+208.*25.4/sqrt(32.0),	// v1 wire 208
  CH_OHIPS_POSITION_V1+209.*25.4/sqrt(32.0),	// v1 wire 209
  CH_OHIPS_POSITION_V1+210.*25.4/sqrt(32.0),	// v1 wire 210
  CH_OHIPS_POSITION_V1+211.*25.4/sqrt(32.0),	// v1 wire 211
  CH_OHIPS_POSITION_V1+212.*25.4/sqrt(32.0),	// v1 wire 212
  CH_OHIPS_POSITION_V1+213.*25.4/sqrt(32.0),	// v1 wire 213
  CH_OHIPS_POSITION_V1+214.*25.4/sqrt(32.0),	// v1 wire 214
  CH_OHIPS_POSITION_V1+215.*25.4/sqrt(32.0),	// v1 wire 215
  CH_OHIPS_POSITION_V1+216.*25.4/sqrt(32.0),	// v1 wire 216
  CH_OHIPS_POSITION_V1+217.*25.4/sqrt(32.0),	// v1 wire 217
  CH_OHIPS_POSITION_V1+218.*25.4/sqrt(32.0),	// v1 wire 218
  CH_OHIPS_POSITION_V1+219.*25.4/sqrt(32.0),	// v1 wire 219
  CH_OHIPS_POSITION_V1+220.*25.4/sqrt(32.0),	// v1 wire 220
  CH_OHIPS_POSITION_V1+221.*25.4/sqrt(32.0),	// v1 wire 221
  CH_OHIPS_POSITION_V1+222.*25.4/sqrt(32.0),	// v1 wire 222
  CH_OHIPS_POSITION_V1+223.*25.4/sqrt(32.0),	// v1 wire 223
  CH_OHIPS_POSITION_V1+224.*25.4/sqrt(32.0),	// v1 wire 224
  CH_OHIPS_POSITION_V1+225.*25.4/sqrt(32.0),	// v1 wire 225
  CH_OHIPS_POSITION_V1+226.*25.4/sqrt(32.0),	// v1 wire 226
  CH_OHIPS_POSITION_V1+227.*25.4/sqrt(32.0),	// v1 wire 227
  CH_OHIPS_POSITION_V1+228.*25.4/sqrt(32.0),	// v1 wire 228
  CH_OHIPS_POSITION_V1+229.*25.4/sqrt(32.0),	// v1 wire 229
  CH_OHIPS_POSITION_V1+230.*25.4/sqrt(32.0),	// v1 wire 230
  CH_OHIPS_POSITION_V1+231.*25.4/sqrt(32.0),	// v1 wire 231
  CH_OHIPS_POSITION_V1+232.*25.4/sqrt(32.0),	// v1 wire 232
  CH_OHIPS_POSITION_V1+233.*25.4/sqrt(32.0),	// v1 wire 233
  CH_OHIPS_POSITION_V1+234.*25.4/sqrt(32.0),	// v1 wire 234
  CH_OHIPS_POSITION_V1+235.*25.4/sqrt(32.0),	// v1 wire 235
  CH_OHIPS_POSITION_V1+236.*25.4/sqrt(32.0),	// v1 wire 236
  CH_OHIPS_POSITION_V1+237.*25.4/sqrt(32.0),	// v1 wire 237
  CH_OHIPS_POSITION_V1+238.*25.4/sqrt(32.0),	// v1 wire 238
  CH_OHIPS_POSITION_V1+239.*25.4/sqrt(32.0),	// v1 wire 239
  CH_OHIPS_POSITION_V1+240.*25.4/sqrt(32.0),	// v1 wire 240
  CH_OHIPS_POSITION_V1+241.*25.4/sqrt(32.0),	// v1 wire 241
  CH_OHIPS_POSITION_V1+242.*25.4/sqrt(32.0),	// v1 wire 242
  CH_OHIPS_POSITION_V1+243.*25.4/sqrt(32.0),	// v1 wire 243
  CH_OHIPS_POSITION_V1+244.*25.4/sqrt(32.0),	// v1 wire 244
  CH_OHIPS_POSITION_V1+245.*25.4/sqrt(32.0),	// v1 wire 245
  CH_OHIPS_POSITION_V1+246.*25.4/sqrt(32.0),	// v1 wire 246
  CH_OHIPS_POSITION_V1+247.*25.4/sqrt(32.0),	// v1 wire 247
  CH_OHIPS_POSITION_V1+248.*25.4/sqrt(32.0),	// v1 wire 248
  CH_OHIPS_POSITION_V1+249.*25.4/sqrt(32.0),	// v1 wire 249
  CH_OHIPS_POSITION_V1+250.*25.4/sqrt(32.0),	// v1 wire 250
  CH_OHIPS_POSITION_V1+251.*25.4/sqrt(32.0),	// v1 wire 251
  CH_OHIPS_POSITION_V1+252.*25.4/sqrt(32.0),	// v1 wire 252
  CH_OHIPS_POSITION_V1+253.*25.4/sqrt(32.0),	// v1 wire 253
  CH_OHIPS_POSITION_V1+254.*25.4/sqrt(32.0),	// v1 wire 254
  CH_OHIPS_POSITION_V1+255.*25.4/sqrt(32.0),	// v1 wire 255
  CH_OHIPS_POSITION_V1+256.*25.4/sqrt(32.0),	// v1 wire 256
};

static float
position_ou2[CH_OHIPS_MAX_U2] = {
  CH_OHIPS_POSITION_U2+1.*sqrt(18.),	// u2 wire 1
  CH_OHIPS_POSITION_U2+2.*sqrt(18.),	// u2 wire 2
  CH_OHIPS_POSITION_U2+3.*sqrt(18.),	// u2 wire 3
  CH_OHIPS_POSITION_U2+4.*sqrt(18.),	// u2 wire 4
  CH_OHIPS_POSITION_U2+5.*sqrt(18.),	// u2 wire 5
  CH_OHIPS_POSITION_U2+6.*sqrt(18.),	// u2 wire 6
  CH_OHIPS_POSITION_U2+7.*sqrt(18.),	// u2 wire 7
  CH_OHIPS_POSITION_U2+8.*sqrt(18.),	// u2 wire 8
  CH_OHIPS_POSITION_U2+9.*sqrt(18.),	// u2 wire 9
  CH_OHIPS_POSITION_U2+10.*sqrt(18.),	// u2 wire 10
  CH_OHIPS_POSITION_U2+11.*sqrt(18.),	// u2 wire 11
  CH_OHIPS_POSITION_U2+12.*sqrt(18.),	// u2 wire 12
  CH_OHIPS_POSITION_U2+13.*sqrt(18.),	// u2 wire 13
  CH_OHIPS_POSITION_U2+14.*sqrt(18.),	// u2 wire 14
  CH_OHIPS_POSITION_U2+15.*sqrt(18.),	// u2 wire 15
  CH_OHIPS_POSITION_U2+16.*sqrt(18.),	// u2 wire 16
  CH_OHIPS_POSITION_U2+17.*sqrt(18.),	// u2 wire 17
  CH_OHIPS_POSITION_U2+18.*sqrt(18.),	// u2 wire 18
  CH_OHIPS_POSITION_U2+19.*sqrt(18.),	// u2 wire 19
  CH_OHIPS_POSITION_U2+20.*sqrt(18.),	// u2 wire 20
  CH_OHIPS_POSITION_U2+21.*sqrt(18.),	// u2 wire 21
  CH_OHIPS_POSITION_U2+22.*sqrt(18.),	// u2 wire 22
  CH_OHIPS_POSITION_U2+23.*sqrt(18.),	// u2 wire 23
  CH_OHIPS_POSITION_U2+24.*sqrt(18.),	// u2 wire 24
  CH_OHIPS_POSITION_U2+25.*sqrt(18.),	// u2 wire 25
  CH_OHIPS_POSITION_U2+26.*sqrt(18.),	// u2 wire 26
  CH_OHIPS_POSITION_U2+27.*sqrt(18.),	// u2 wire 27
  CH_OHIPS_POSITION_U2+28.*sqrt(18.),	// u2 wire 28
  CH_OHIPS_POSITION_U2+29.*sqrt(18.),	// u2 wire 29
  CH_OHIPS_POSITION_U2+30.*sqrt(18.),	// u2 wire 30
  CH_OHIPS_POSITION_U2+31.*sqrt(18.),	// u2 wire 31
  CH_OHIPS_POSITION_U2+32.*sqrt(18.),	// u2 wire 32
  CH_OHIPS_POSITION_U2+33.*sqrt(18.),	// u2 wire 33
  CH_OHIPS_POSITION_U2+34.*sqrt(18.),	// u2 wire 34
  CH_OHIPS_POSITION_U2+35.*sqrt(18.),	// u2 wire 35
  CH_OHIPS_POSITION_U2+36.*sqrt(18.),	// u2 wire 36
  CH_OHIPS_POSITION_U2+37.*sqrt(18.),	// u2 wire 37
  CH_OHIPS_POSITION_U2+38.*sqrt(18.),	// u2 wire 38
  CH_OHIPS_POSITION_U2+39.*sqrt(18.),	// u2 wire 39
  CH_OHIPS_POSITION_U2+40.*sqrt(18.),	// u2 wire 40
  CH_OHIPS_POSITION_U2+41.*sqrt(18.),	// u2 wire 41
  CH_OHIPS_POSITION_U2+42.*sqrt(18.),	// u2 wire 42
  CH_OHIPS_POSITION_U2+43.*sqrt(18.),	// u2 wire 43
  CH_OHIPS_POSITION_U2+44.*sqrt(18.),	// u2 wire 44
  CH_OHIPS_POSITION_U2+45.*sqrt(18.),	// u2 wire 45
  CH_OHIPS_POSITION_U2+46.*sqrt(18.),	// u2 wire 46
  CH_OHIPS_POSITION_U2+47.*sqrt(18.),	// u2 wire 47
  CH_OHIPS_POSITION_U2+48.*sqrt(18.),	// u2 wire 48
  CH_OHIPS_POSITION_U2+49.*sqrt(18.),	// u2 wire 49
  CH_OHIPS_POSITION_U2+50.*sqrt(18.),	// u2 wire 50
  CH_OHIPS_POSITION_U2+51.*sqrt(18.),	// u2 wire 51
  CH_OHIPS_POSITION_U2+52.*sqrt(18.),	// u2 wire 52
  CH_OHIPS_POSITION_U2+53.*sqrt(18.),	// u2 wire 53
  CH_OHIPS_POSITION_U2+54.*sqrt(18.),	// u2 wire 54
  CH_OHIPS_POSITION_U2+55.*sqrt(18.),	// u2 wire 55
  CH_OHIPS_POSITION_U2+56.*sqrt(18.),	// u2 wire 56
  CH_OHIPS_POSITION_U2+57.*sqrt(18.),	// u2 wire 57
  CH_OHIPS_POSITION_U2+58.*sqrt(18.),	// u2 wire 58
  CH_OHIPS_POSITION_U2+59.*sqrt(18.),	// u2 wire 59
  CH_OHIPS_POSITION_U2+60.*sqrt(18.),	// u2 wire 60
  CH_OHIPS_POSITION_U2+61.*sqrt(18.),	// u2 wire 61
  CH_OHIPS_POSITION_U2+62.*sqrt(18.),	// u2 wire 62
  CH_OHIPS_POSITION_U2+63.*sqrt(18.),	// u2 wire 63
  CH_OHIPS_POSITION_U2+64.*sqrt(18.),	// u2 wire 64
  CH_OHIPS_POSITION_U2+65.*sqrt(18.),	// u2 wire 65
  CH_OHIPS_POSITION_U2+66.*sqrt(18.),	// u2 wire 66
  CH_OHIPS_POSITION_U2+67.*sqrt(18.),	// u2 wire 67
  CH_OHIPS_POSITION_U2+68.*sqrt(18.),	// u2 wire 68
  CH_OHIPS_POSITION_U2+69.*sqrt(18.),	// u2 wire 69
  CH_OHIPS_POSITION_U2+70.*sqrt(18.),	// u2 wire 70
  CH_OHIPS_POSITION_U2+71.*sqrt(18.),	// u2 wire 71
  CH_OHIPS_POSITION_U2+72.*sqrt(18.),	// u2 wire 72
  CH_OHIPS_POSITION_U2+73.*sqrt(18.),	// u2 wire 73
  CH_OHIPS_POSITION_U2+74.*sqrt(18.),	// u2 wire 74
  CH_OHIPS_POSITION_U2+75.*sqrt(18.),	// u2 wire 75
  CH_OHIPS_POSITION_U2+76.*sqrt(18.),	// u2 wire 76
  CH_OHIPS_POSITION_U2+77.*sqrt(18.),	// u2 wire 77
  CH_OHIPS_POSITION_U2+78.*sqrt(18.),	// u2 wire 78
  CH_OHIPS_POSITION_U2+79.*sqrt(18.),	// u2 wire 79
  CH_OHIPS_POSITION_U2+80.*sqrt(18.),	// u2 wire 80
  CH_OHIPS_POSITION_U2+81.*sqrt(18.),	// u2 wire 81
  CH_OHIPS_POSITION_U2+82.*sqrt(18.),	// u2 wire 82
  CH_OHIPS_POSITION_U2+83.*sqrt(18.),	// u2 wire 83
  CH_OHIPS_POSITION_U2+84.*sqrt(18.),	// u2 wire 84
  CH_OHIPS_POSITION_U2+85.*sqrt(18.),	// u2 wire 85
  CH_OHIPS_POSITION_U2+86.*sqrt(18.),	// u2 wire 86
  CH_OHIPS_POSITION_U2+87.*sqrt(18.),	// u2 wire 87
  CH_OHIPS_POSITION_U2+88.*sqrt(18.),	// u2 wire 88
  CH_OHIPS_POSITION_U2+89.*sqrt(18.),	// u2 wire 89
  CH_OHIPS_POSITION_U2+90.*sqrt(18.),	// u2 wire 90
  CH_OHIPS_POSITION_U2+91.*sqrt(18.),	// u2 wire 91
  CH_OHIPS_POSITION_U2+92.*sqrt(18.),	// u2 wire 92
  CH_OHIPS_POSITION_U2+93.*sqrt(18.),	// u2 wire 93
  CH_OHIPS_POSITION_U2+94.*sqrt(18.),	// u2 wire 94
  CH_OHIPS_POSITION_U2+95.*sqrt(18.),	// u2 wire 95
  CH_OHIPS_POSITION_U2+96.*sqrt(18.),	// u2 wire 96
  CH_OHIPS_POSITION_U2+97.*sqrt(18.),	// u2 wire 97
  CH_OHIPS_POSITION_U2+98.*sqrt(18.),	// u2 wire 98
  CH_OHIPS_POSITION_U2+99.*sqrt(18.),	// u2 wire 99
  CH_OHIPS_POSITION_U2+100.*sqrt(18.),	// u2 wire 100
  CH_OHIPS_POSITION_U2+101.*sqrt(18.),	// u2 wire 101
  CH_OHIPS_POSITION_U2+102.*sqrt(18.),	// u2 wire 102
  CH_OHIPS_POSITION_U2+103.*sqrt(18.),	// u2 wire 103
  CH_OHIPS_POSITION_U2+104.*sqrt(18.),	// u2 wire 104
  CH_OHIPS_POSITION_U2+105.*sqrt(18.),	// u2 wire 105
  CH_OHIPS_POSITION_U2+106.*sqrt(18.),	// u2 wire 106
  CH_OHIPS_POSITION_U2+107.*sqrt(18.),	// u2 wire 107
  CH_OHIPS_POSITION_U2+108.*sqrt(18.),	// u2 wire 108
  CH_OHIPS_POSITION_U2+109.*sqrt(18.),	// u2 wire 109
  CH_OHIPS_POSITION_U2+110.*sqrt(18.),	// u2 wire 110
  CH_OHIPS_POSITION_U2+111.*sqrt(18.),	// u2 wire 111
  CH_OHIPS_POSITION_U2+112.*sqrt(18.),	// u2 wire 112
  CH_OHIPS_POSITION_U2+113.*sqrt(18.),	// u2 wire 113
  CH_OHIPS_POSITION_U2+114.*sqrt(18.),	// u2 wire 114
  CH_OHIPS_POSITION_U2+115.*sqrt(18.),	// u2 wire 115
  CH_OHIPS_POSITION_U2+116.*sqrt(18.),	// u2 wire 116
  CH_OHIPS_POSITION_U2+117.*sqrt(18.),	// u2 wire 117
  CH_OHIPS_POSITION_U2+118.*sqrt(18.),	// u2 wire 118
  CH_OHIPS_POSITION_U2+119.*sqrt(18.),	// u2 wire 119
  CH_OHIPS_POSITION_U2+120.*sqrt(18.),	// u2 wire 120
  CH_OHIPS_POSITION_U2+121.*sqrt(18.),	// u2 wire 121
  CH_OHIPS_POSITION_U2+122.*sqrt(18.),	// u2 wire 122
  CH_OHIPS_POSITION_U2+123.*sqrt(18.),	// u2 wire 123
  CH_OHIPS_POSITION_U2+124.*sqrt(18.),	// u2 wire 124
  CH_OHIPS_POSITION_U2+125.*sqrt(18.),	// u2 wire 125
  CH_OHIPS_POSITION_U2+126.*sqrt(18.),	// u2 wire 126
  CH_OHIPS_POSITION_U2+127.*sqrt(18.),	// u2 wire 127
  CH_OHIPS_POSITION_U2+128.*sqrt(18.),	// u2 wire 128
  CH_OHIPS_POSITION_U2+129.*sqrt(18.),	// u2 wire 129
  CH_OHIPS_POSITION_U2+130.*sqrt(18.),	// u2 wire 130
  CH_OHIPS_POSITION_U2+131.*sqrt(18.),	// u2 wire 131
  CH_OHIPS_POSITION_U2+132.*sqrt(18.),	// u2 wire 132
  CH_OHIPS_POSITION_U2+133.*sqrt(18.),	// u2 wire 133
  CH_OHIPS_POSITION_U2+134.*sqrt(18.),	// u2 wire 134
  CH_OHIPS_POSITION_U2+135.*sqrt(18.),	// u2 wire 135
  CH_OHIPS_POSITION_U2+136.*sqrt(18.),	// u2 wire 136
  CH_OHIPS_POSITION_U2+137.*sqrt(18.),	// u2 wire 137
  CH_OHIPS_POSITION_U2+138.*sqrt(18.),	// u2 wire 138
  CH_OHIPS_POSITION_U2+139.*sqrt(18.),	// u2 wire 139
  CH_OHIPS_POSITION_U2+140.*sqrt(18.),	// u2 wire 140
  CH_OHIPS_POSITION_U2+141.*sqrt(18.),	// u2 wire 141
  CH_OHIPS_POSITION_U2+142.*sqrt(18.),	// u2 wire 142
  CH_OHIPS_POSITION_U2+143.*sqrt(18.),	// u2 wire 143
  CH_OHIPS_POSITION_U2+144.*sqrt(18.),	// u2 wire 144
  CH_OHIPS_POSITION_U2+145.*sqrt(18.),	// u2 wire 145
  CH_OHIPS_POSITION_U2+146.*sqrt(18.),	// u2 wire 146
  CH_OHIPS_POSITION_U2+147.*sqrt(18.),	// u2 wire 147
  CH_OHIPS_POSITION_U2+148.*sqrt(18.),	// u2 wire 148
  CH_OHIPS_POSITION_U2+149.*sqrt(18.),	// u2 wire 149
  CH_OHIPS_POSITION_U2+150.*sqrt(18.),	// u2 wire 150
  CH_OHIPS_POSITION_U2+151.*sqrt(18.),	// u2 wire 151
  CH_OHIPS_POSITION_U2+152.*sqrt(18.),	// u2 wire 152
  CH_OHIPS_POSITION_U2+153.*sqrt(18.),	// u2 wire 153
  CH_OHIPS_POSITION_U2+154.*sqrt(18.),	// u2 wire 154
  CH_OHIPS_POSITION_U2+155.*sqrt(18.),	// u2 wire 155
  CH_OHIPS_POSITION_U2+156.*sqrt(18.),	// u2 wire 156
  CH_OHIPS_POSITION_U2+157.*sqrt(18.),	// u2 wire 157
  CH_OHIPS_POSITION_U2+158.*sqrt(18.),	// u2 wire 158
  CH_OHIPS_POSITION_U2+159.*sqrt(18.),	// u2 wire 159
  CH_OHIPS_POSITION_U2+160.*sqrt(18.),	// u2 wire 160
  CH_OHIPS_POSITION_U2+161.*sqrt(18.),	// u2 wire 161
  CH_OHIPS_POSITION_U2+162.*sqrt(18.),	// u2 wire 162
  CH_OHIPS_POSITION_U2+163.*sqrt(18.),	// u2 wire 163
  CH_OHIPS_POSITION_U2+164.*sqrt(18.),	// u2 wire 164
  CH_OHIPS_POSITION_U2+165.*sqrt(18.),	// u2 wire 165
  CH_OHIPS_POSITION_U2+166.*sqrt(18.),	// u2 wire 166
  CH_OHIPS_POSITION_U2+167.*sqrt(18.),	// u2 wire 167
  CH_OHIPS_POSITION_U2+168.*sqrt(18.),	// u2 wire 168
  CH_OHIPS_POSITION_U2+169.*sqrt(18.),	// u2 wire 169
  CH_OHIPS_POSITION_U2+170.*sqrt(18.),	// u2 wire 170
  CH_OHIPS_POSITION_U2+171.*sqrt(18.),	// u2 wire 171
  CH_OHIPS_POSITION_U2+172.*sqrt(18.),	// u2 wire 172
  CH_OHIPS_POSITION_U2+173.*sqrt(18.),	// u2 wire 173
  CH_OHIPS_POSITION_U2+174.*sqrt(18.),	// u2 wire 174
  CH_OHIPS_POSITION_U2+175.*sqrt(18.),	// u2 wire 175
  CH_OHIPS_POSITION_U2+176.*sqrt(18.),	// u2 wire 176
  CH_OHIPS_POSITION_U2+177.*sqrt(18.),	// u2 wire 177
  CH_OHIPS_POSITION_U2+178.*sqrt(18.),	// u2 wire 178
  CH_OHIPS_POSITION_U2+179.*sqrt(18.),	// u2 wire 179
  CH_OHIPS_POSITION_U2+180.*sqrt(18.),	// u2 wire 180
  CH_OHIPS_POSITION_U2+181.*sqrt(18.),	// u2 wire 181
  CH_OHIPS_POSITION_U2+182.*sqrt(18.),	// u2 wire 182
  CH_OHIPS_POSITION_U2+183.*sqrt(18.),	// u2 wire 183
  CH_OHIPS_POSITION_U2+184.*sqrt(18.),	// u2 wire 184
  CH_OHIPS_POSITION_U2+185.*sqrt(18.),	// u2 wire 185
  CH_OHIPS_POSITION_U2+186.*sqrt(18.),	// u2 wire 186
  CH_OHIPS_POSITION_U2+187.*sqrt(18.),	// u2 wire 187
  CH_OHIPS_POSITION_U2+188.*sqrt(18.),	// u2 wire 188
  CH_OHIPS_POSITION_U2+189.*sqrt(18.),	// u2 wire 189
  CH_OHIPS_POSITION_U2+190.*sqrt(18.),	// u2 wire 190
  CH_OHIPS_POSITION_U2+191.*sqrt(18.),	// u2 wire 191
  CH_OHIPS_POSITION_U2+192.*sqrt(18.),	// u2 wire 192
  CH_OHIPS_POSITION_U2+193.*sqrt(18.),	// u2 wire 193
  CH_OHIPS_POSITION_U2+194.*sqrt(18.),	// u2 wire 194
  CH_OHIPS_POSITION_U2+195.*sqrt(18.),	// u2 wire 195
  CH_OHIPS_POSITION_U2+196.*sqrt(18.),	// u2 wire 196
  CH_OHIPS_POSITION_U2+197.*sqrt(18.),	// u2 wire 197
  CH_OHIPS_POSITION_U2+198.*sqrt(18.),	// u2 wire 198
  CH_OHIPS_POSITION_U2+199.*sqrt(18.),	// u2 wire 199
  CH_OHIPS_POSITION_U2+200.*sqrt(18.),	// u2 wire 200
  CH_OHIPS_POSITION_U2+201.*sqrt(18.),	// u2 wire 201
  CH_OHIPS_POSITION_U2+202.*sqrt(18.),	// u2 wire 202
  CH_OHIPS_POSITION_U2+203.*sqrt(18.),	// u2 wire 203
  CH_OHIPS_POSITION_U2+204.*sqrt(18.),	// u2 wire 204
  CH_OHIPS_POSITION_U2+205.*sqrt(18.),	// u2 wire 205
  CH_OHIPS_POSITION_U2+206.*sqrt(18.),	// u2 wire 206
  CH_OHIPS_POSITION_U2+207.*sqrt(18.),	// u2 wire 207
  CH_OHIPS_POSITION_U2+208.*sqrt(18.),	// u2 wire 208
  CH_OHIPS_POSITION_U2+209.*sqrt(18.),	// u2 wire 209
  CH_OHIPS_POSITION_U2+210.*sqrt(18.),	// u2 wire 210
  CH_OHIPS_POSITION_U2+211.*sqrt(18.),	// u2 wire 211
  CH_OHIPS_POSITION_U2+212.*sqrt(18.),	// u2 wire 212
  CH_OHIPS_POSITION_U2+213.*sqrt(18.),	// u2 wire 213
  CH_OHIPS_POSITION_U2+214.*sqrt(18.),	// u2 wire 214
  CH_OHIPS_POSITION_U2+215.*sqrt(18.),	// u2 wire 215
  CH_OHIPS_POSITION_U2+216.*sqrt(18.),	// u2 wire 216
  CH_OHIPS_POSITION_U2+217.*sqrt(18.),	// u2 wire 217
  CH_OHIPS_POSITION_U2+218.*sqrt(18.),	// u2 wire 218
  CH_OHIPS_POSITION_U2+219.*sqrt(18.),	// u2 wire 219
  CH_OHIPS_POSITION_U2+220.*sqrt(18.),	// u2 wire 220
  CH_OHIPS_POSITION_U2+221.*sqrt(18.),	// u2 wire 221
  CH_OHIPS_POSITION_U2+222.*sqrt(18.),	// u2 wire 222
  CH_OHIPS_POSITION_U2+223.*sqrt(18.),	// u2 wire 223
  CH_OHIPS_POSITION_U2+224.*sqrt(18.),	// u2 wire 224
  CH_OHIPS_POSITION_U2+225.*sqrt(18.),	// u2 wire 225
  CH_OHIPS_POSITION_U2+226.*sqrt(18.),	// u2 wire 226
  CH_OHIPS_POSITION_U2+227.*sqrt(18.),	// u2 wire 227
  CH_OHIPS_POSITION_U2+228.*sqrt(18.),	// u2 wire 228
  CH_OHIPS_POSITION_U2+229.*sqrt(18.),	// u2 wire 229
  CH_OHIPS_POSITION_U2+230.*sqrt(18.),	// u2 wire 230
  CH_OHIPS_POSITION_U2+231.*sqrt(18.),	// u2 wire 231
  CH_OHIPS_POSITION_U2+232.*sqrt(18.),	// u2 wire 232
  CH_OHIPS_POSITION_U2+233.*sqrt(18.),	// u2 wire 233
  CH_OHIPS_POSITION_U2+234.*sqrt(18.),	// u2 wire 234
  CH_OHIPS_POSITION_U2+235.*sqrt(18.),	// u2 wire 235
  CH_OHIPS_POSITION_U2+236.*sqrt(18.),	// u2 wire 236
  CH_OHIPS_POSITION_U2+237.*sqrt(18.),	// u2 wire 237
  CH_OHIPS_POSITION_U2+238.*sqrt(18.),	// u2 wire 238
  CH_OHIPS_POSITION_U2+239.*sqrt(18.),	// u2 wire 239
  CH_OHIPS_POSITION_U2+240.*sqrt(18.),	// u2 wire 240
  CH_OHIPS_POSITION_U2+241.*sqrt(18.),	// u2 wire 241
  CH_OHIPS_POSITION_U2+242.*sqrt(18.),	// u2 wire 242
  CH_OHIPS_POSITION_U2+243.*sqrt(18.),	// u2 wire 243
  CH_OHIPS_POSITION_U2+244.*sqrt(18.),	// u2 wire 244
  CH_OHIPS_POSITION_U2+245.*sqrt(18.),	// u2 wire 245
  CH_OHIPS_POSITION_U2+246.*sqrt(18.),	// u2 wire 246
  CH_OHIPS_POSITION_U2+247.*sqrt(18.),	// u2 wire 247
  CH_OHIPS_POSITION_U2+248.*sqrt(18.),	// u2 wire 248
  CH_OHIPS_POSITION_U2+249.*sqrt(18.),	// u2 wire 249
  CH_OHIPS_POSITION_U2+250.*sqrt(18.),	// u2 wire 250
  CH_OHIPS_POSITION_U2+251.*sqrt(18.),	// u2 wire 251
  CH_OHIPS_POSITION_U2+252.*sqrt(18.),	// u2 wire 252
  CH_OHIPS_POSITION_U2+253.*sqrt(18.),	// u2 wire 253
  CH_OHIPS_POSITION_U2+254.*sqrt(18.),	// u2 wire 254
  CH_OHIPS_POSITION_U2+255.*sqrt(18.),	// u2 wire 255
  CH_OHIPS_POSITION_U2+256.*sqrt(18.)	// u2 wire 256
};


static float
position_ov2[CH_OHIPS_MAX_V2] = {
  CH_OHIPS_POSITION_V2+1.*sqrt(18.),	// v2 wire 1
  CH_OHIPS_POSITION_V2+2.*sqrt(18.),	// v2 wire 2
  CH_OHIPS_POSITION_V2+3.*sqrt(18.),	// v2 wire 3
  CH_OHIPS_POSITION_V2+4.*sqrt(18.),	// v2 wire 4
  CH_OHIPS_POSITION_V2+5.*sqrt(18.),	// v2 wire 5
  CH_OHIPS_POSITION_V2+6.*sqrt(18.),	// v2 wire 6
  CH_OHIPS_POSITION_V2+7.*sqrt(18.),	// v2 wire 7
  CH_OHIPS_POSITION_V2+8.*sqrt(18.),	// v2 wire 8
  CH_OHIPS_POSITION_V2+9.*sqrt(18.),	// v2 wire 9
  CH_OHIPS_POSITION_V2+10.*sqrt(18.),	// v2 wire 10
  CH_OHIPS_POSITION_V2+11.*sqrt(18.),	// v2 wire 11
  CH_OHIPS_POSITION_V2+12.*sqrt(18.),	// v2 wire 12
  CH_OHIPS_POSITION_V2+13.*sqrt(18.),	// v2 wire 13
  CH_OHIPS_POSITION_V2+14.*sqrt(18.),	// v2 wire 14
  CH_OHIPS_POSITION_V2+15.*sqrt(18.),	// v2 wire 15
  CH_OHIPS_POSITION_V2+16.*sqrt(18.),	// v2 wire 16
  CH_OHIPS_POSITION_V2+17.*sqrt(18.),	// v2 wire 17
  CH_OHIPS_POSITION_V2+18.*sqrt(18.),	// v2 wire 18
  CH_OHIPS_POSITION_V2+19.*sqrt(18.),	// v2 wire 19
  CH_OHIPS_POSITION_V2+20.*sqrt(18.),	// v2 wire 20
  CH_OHIPS_POSITION_V2+21.*sqrt(18.),	// v2 wire 21
  CH_OHIPS_POSITION_V2+22.*sqrt(18.),	// v2 wire 22
  CH_OHIPS_POSITION_V2+23.*sqrt(18.),	// v2 wire 23
  CH_OHIPS_POSITION_V2+24.*sqrt(18.),	// v2 wire 24
  CH_OHIPS_POSITION_V2+25.*sqrt(18.),	// v2 wire 25
  CH_OHIPS_POSITION_V2+26.*sqrt(18.),	// v2 wire 26
  CH_OHIPS_POSITION_V2+27.*sqrt(18.),	// v2 wire 27
  CH_OHIPS_POSITION_V2+28.*sqrt(18.),	// v2 wire 28
  CH_OHIPS_POSITION_V2+29.*sqrt(18.),	// v2 wire 29
  CH_OHIPS_POSITION_V2+30.*sqrt(18.),	// v2 wire 30
  CH_OHIPS_POSITION_V2+31.*sqrt(18.),	// v2 wire 31
  CH_OHIPS_POSITION_V2+32.*sqrt(18.),	// v2 wire 32
  CH_OHIPS_POSITION_V2+33.*sqrt(18.),	// v2 wire 33
  CH_OHIPS_POSITION_V2+34.*sqrt(18.),	// v2 wire 34
  CH_OHIPS_POSITION_V2+35.*sqrt(18.),	// v2 wire 35
  CH_OHIPS_POSITION_V2+36.*sqrt(18.),	// v2 wire 36
  CH_OHIPS_POSITION_V2+37.*sqrt(18.),	// v2 wire 37
  CH_OHIPS_POSITION_V2+38.*sqrt(18.),	// v2 wire 38
  CH_OHIPS_POSITION_V2+39.*sqrt(18.),	// v2 wire 39
  CH_OHIPS_POSITION_V2+40.*sqrt(18.),	// v2 wire 40
  CH_OHIPS_POSITION_V2+41.*sqrt(18.),	// v2 wire 41
  CH_OHIPS_POSITION_V2+42.*sqrt(18.),	// v2 wire 42
  CH_OHIPS_POSITION_V2+43.*sqrt(18.),	// v2 wire 43
  CH_OHIPS_POSITION_V2+44.*sqrt(18.),	// v2 wire 44
  CH_OHIPS_POSITION_V2+45.*sqrt(18.),	// v2 wire 45
  CH_OHIPS_POSITION_V2+46.*sqrt(18.),	// v2 wire 46
  CH_OHIPS_POSITION_V2+47.*sqrt(18.),	// v2 wire 47
  CH_OHIPS_POSITION_V2+48.*sqrt(18.),	// v2 wire 48
  CH_OHIPS_POSITION_V2+49.*sqrt(18.),	// v2 wire 49
  CH_OHIPS_POSITION_V2+50.*sqrt(18.),	// v2 wire 50
  CH_OHIPS_POSITION_V2+51.*sqrt(18.),	// v2 wire 51
  CH_OHIPS_POSITION_V2+52.*sqrt(18.),	// v2 wire 52
  CH_OHIPS_POSITION_V2+53.*sqrt(18.),	// v2 wire 53
  CH_OHIPS_POSITION_V2+54.*sqrt(18.),	// v2 wire 54
  CH_OHIPS_POSITION_V2+55.*sqrt(18.),	// v2 wire 55
  CH_OHIPS_POSITION_V2+56.*sqrt(18.),	// v2 wire 56
  CH_OHIPS_POSITION_V2+57.*sqrt(18.),	// v2 wire 57
  CH_OHIPS_POSITION_V2+58.*sqrt(18.),	// v2 wire 58
  CH_OHIPS_POSITION_V2+59.*sqrt(18.),	// v2 wire 59
  CH_OHIPS_POSITION_V2+60.*sqrt(18.),	// v2 wire 60
  CH_OHIPS_POSITION_V2+61.*sqrt(18.),	// v2 wire 61
  CH_OHIPS_POSITION_V2+62.*sqrt(18.),	// v2 wire 62
  CH_OHIPS_POSITION_V2+63.*sqrt(18.),	// v2 wire 63
  CH_OHIPS_POSITION_V2+64.*sqrt(18.),	// v2 wire 64
  CH_OHIPS_POSITION_V2+65.*sqrt(18.),	// v2 wire 65
  CH_OHIPS_POSITION_V2+66.*sqrt(18.),	// v2 wire 66
  CH_OHIPS_POSITION_V2+67.*sqrt(18.),	// v2 wire 67
  CH_OHIPS_POSITION_V2+68.*sqrt(18.),	// v2 wire 68
  CH_OHIPS_POSITION_V2+69.*sqrt(18.),	// v2 wire 69
  CH_OHIPS_POSITION_V2+70.*sqrt(18.),	// v2 wire 70
  CH_OHIPS_POSITION_V2+71.*sqrt(18.),	// v2 wire 71
  CH_OHIPS_POSITION_V2+72.*sqrt(18.),	// v2 wire 72
  CH_OHIPS_POSITION_V2+73.*sqrt(18.),	// v2 wire 73
  CH_OHIPS_POSITION_V2+74.*sqrt(18.),	// v2 wire 74
  CH_OHIPS_POSITION_V2+75.*sqrt(18.),	// v2 wire 75
  CH_OHIPS_POSITION_V2+76.*sqrt(18.),	// v2 wire 76
  CH_OHIPS_POSITION_V2+77.*sqrt(18.),	// v2 wire 77
  CH_OHIPS_POSITION_V2+78.*sqrt(18.),	// v2 wire 78
  CH_OHIPS_POSITION_V2+79.*sqrt(18.),	// v2 wire 79
  CH_OHIPS_POSITION_V2+80.*sqrt(18.),	// v2 wire 80
  CH_OHIPS_POSITION_V2+81.*sqrt(18.),	// v2 wire 81
  CH_OHIPS_POSITION_V2+82.*sqrt(18.),	// v2 wire 82
  CH_OHIPS_POSITION_V2+83.*sqrt(18.),	// v2 wire 83
  CH_OHIPS_POSITION_V2+84.*sqrt(18.),	// v2 wire 84
  CH_OHIPS_POSITION_V2+85.*sqrt(18.),	// v2 wire 85
  CH_OHIPS_POSITION_V2+86.*sqrt(18.),	// v2 wire 86
  CH_OHIPS_POSITION_V2+87.*sqrt(18.),	// v2 wire 87
  CH_OHIPS_POSITION_V2+88.*sqrt(18.),	// v2 wire 88
  CH_OHIPS_POSITION_V2+89.*sqrt(18.),	// v2 wire 89
  CH_OHIPS_POSITION_V2+90.*sqrt(18.),	// v2 wire 90
  CH_OHIPS_POSITION_V2+91.*sqrt(18.),	// v2 wire 91
  CH_OHIPS_POSITION_V2+92.*sqrt(18.),	// v2 wire 92
  CH_OHIPS_POSITION_V2+93.*sqrt(18.),	// v2 wire 93
  CH_OHIPS_POSITION_V2+94.*sqrt(18.),	// v2 wire 94
  CH_OHIPS_POSITION_V2+95.*sqrt(18.),	// v2 wire 95
  CH_OHIPS_POSITION_V2+96.*sqrt(18.),	// v2 wire 96
  CH_OHIPS_POSITION_V2+97.*sqrt(18.),	// v2 wire 97
  CH_OHIPS_POSITION_V2+98.*sqrt(18.),	// v2 wire 98
  CH_OHIPS_POSITION_V2+99.*sqrt(18.),	// v2 wire 99
  CH_OHIPS_POSITION_V2+100.*sqrt(18.),	// v2 wire 100
  CH_OHIPS_POSITION_V2+101.*sqrt(18.),	// v2 wire 101
  CH_OHIPS_POSITION_V2+102.*sqrt(18.),	// v2 wire 102
  CH_OHIPS_POSITION_V2+103.*sqrt(18.),	// v2 wire 103
  CH_OHIPS_POSITION_V2+104.*sqrt(18.),	// v2 wire 104
  CH_OHIPS_POSITION_V2+105.*sqrt(18.),	// v2 wire 105
  CH_OHIPS_POSITION_V2+106.*sqrt(18.),	// v2 wire 106
  CH_OHIPS_POSITION_V2+107.*sqrt(18.),	// v2 wire 107
  CH_OHIPS_POSITION_V2+108.*sqrt(18.),	// v2 wire 108
  CH_OHIPS_POSITION_V2+109.*sqrt(18.),	// v2 wire 109
  CH_OHIPS_POSITION_V2+110.*sqrt(18.),	// v2 wire 110
  CH_OHIPS_POSITION_V2+111.*sqrt(18.),	// v2 wire 111
  CH_OHIPS_POSITION_V2+112.*sqrt(18.),	// v2 wire 112
  CH_OHIPS_POSITION_V2+113.*sqrt(18.),	// v2 wire 113
  CH_OHIPS_POSITION_V2+114.*sqrt(18.),	// v2 wire 114
  CH_OHIPS_POSITION_V2+115.*sqrt(18.),	// v2 wire 115
  CH_OHIPS_POSITION_V2+116.*sqrt(18.),	// v2 wire 116
  CH_OHIPS_POSITION_V2+117.*sqrt(18.),	// v2 wire 117
  CH_OHIPS_POSITION_V2+118.*sqrt(18.),	// v2 wire 118
  CH_OHIPS_POSITION_V2+119.*sqrt(18.),	// v2 wire 119
  CH_OHIPS_POSITION_V2+120.*sqrt(18.),	// v2 wire 120
  CH_OHIPS_POSITION_V2+121.*sqrt(18.),	// v2 wire 121
  CH_OHIPS_POSITION_V2+122.*sqrt(18.),	// v2 wire 122
  CH_OHIPS_POSITION_V2+123.*sqrt(18.),	// v2 wire 123
  CH_OHIPS_POSITION_V2+124.*sqrt(18.),	// v2 wire 124
  CH_OHIPS_POSITION_V2+125.*sqrt(18.),	// v2 wire 125
  CH_OHIPS_POSITION_V2+126.*sqrt(18.),	// v2 wire 126
  CH_OHIPS_POSITION_V2+127.*sqrt(18.),	// v2 wire 127
  CH_OHIPS_POSITION_V2+128.*sqrt(18.),	// v2 wire 128
  CH_OHIPS_POSITION_V2+129.*sqrt(18.),	// v2 wire 129
  CH_OHIPS_POSITION_V2+130.*sqrt(18.),	// v2 wire 130
  CH_OHIPS_POSITION_V2+131.*sqrt(18.),	// v2 wire 131
  CH_OHIPS_POSITION_V2+132.*sqrt(18.),	// v2 wire 132
  CH_OHIPS_POSITION_V2+133.*sqrt(18.),	// v2 wire 133
  CH_OHIPS_POSITION_V2+134.*sqrt(18.),	// v2 wire 134
  CH_OHIPS_POSITION_V2+135.*sqrt(18.),	// v2 wire 135
  CH_OHIPS_POSITION_V2+136.*sqrt(18.),	// v2 wire 136
  CH_OHIPS_POSITION_V2+137.*sqrt(18.),	// v2 wire 137
  CH_OHIPS_POSITION_V2+138.*sqrt(18.),	// v2 wire 138
  CH_OHIPS_POSITION_V2+139.*sqrt(18.),	// v2 wire 139
  CH_OHIPS_POSITION_V2+140.*sqrt(18.),	// v2 wire 140
  CH_OHIPS_POSITION_V2+141.*sqrt(18.),	// v2 wire 141
  CH_OHIPS_POSITION_V2+142.*sqrt(18.),	// v2 wire 142
  CH_OHIPS_POSITION_V2+143.*sqrt(18.),	// v2 wire 143
  CH_OHIPS_POSITION_V2+144.*sqrt(18.),	// v2 wire 144
  CH_OHIPS_POSITION_V2+145.*sqrt(18.),	// v2 wire 145
  CH_OHIPS_POSITION_V2+146.*sqrt(18.),	// v2 wire 146
  CH_OHIPS_POSITION_V2+147.*sqrt(18.),	// v2 wire 147
  CH_OHIPS_POSITION_V2+148.*sqrt(18.),	// v2 wire 148
  CH_OHIPS_POSITION_V2+149.*sqrt(18.),	// v2 wire 149
  CH_OHIPS_POSITION_V2+150.*sqrt(18.),	// v2 wire 150
  CH_OHIPS_POSITION_V2+151.*sqrt(18.),	// v2 wire 151
  CH_OHIPS_POSITION_V2+152.*sqrt(18.),	// v2 wire 152
  CH_OHIPS_POSITION_V2+153.*sqrt(18.),	// v2 wire 153
  CH_OHIPS_POSITION_V2+154.*sqrt(18.),	// v2 wire 154
  CH_OHIPS_POSITION_V2+155.*sqrt(18.),	// v2 wire 155
  CH_OHIPS_POSITION_V2+156.*sqrt(18.),	// v2 wire 156
  CH_OHIPS_POSITION_V2+157.*sqrt(18.),	// v2 wire 157
  CH_OHIPS_POSITION_V2+158.*sqrt(18.),	// v2 wire 158
  CH_OHIPS_POSITION_V2+159.*sqrt(18.),	// v2 wire 159
  CH_OHIPS_POSITION_V2+160.*sqrt(18.),	// v2 wire 160
  CH_OHIPS_POSITION_V2+161.*sqrt(18.),	// v2 wire 161
  CH_OHIPS_POSITION_V2+162.*sqrt(18.),	// v2 wire 162
  CH_OHIPS_POSITION_V2+163.*sqrt(18.),	// v2 wire 163
  CH_OHIPS_POSITION_V2+164.*sqrt(18.),	// v2 wire 164
  CH_OHIPS_POSITION_V2+165.*sqrt(18.),	// v2 wire 165
  CH_OHIPS_POSITION_V2+166.*sqrt(18.),	// v2 wire 166
  CH_OHIPS_POSITION_V2+167.*sqrt(18.),	// v2 wire 167
  CH_OHIPS_POSITION_V2+168.*sqrt(18.),	// v2 wire 168
  CH_OHIPS_POSITION_V2+169.*sqrt(18.),	// v2 wire 169
  CH_OHIPS_POSITION_V2+170.*sqrt(18.),	// v2 wire 170
  CH_OHIPS_POSITION_V2+171.*sqrt(18.),	// v2 wire 171
  CH_OHIPS_POSITION_V2+172.*sqrt(18.),	// v2 wire 172
  CH_OHIPS_POSITION_V2+173.*sqrt(18.),	// v2 wire 173
  CH_OHIPS_POSITION_V2+174.*sqrt(18.),	// v2 wire 174
  CH_OHIPS_POSITION_V2+175.*sqrt(18.),	// v2 wire 175
  CH_OHIPS_POSITION_V2+176.*sqrt(18.),	// v2 wire 176
  CH_OHIPS_POSITION_V2+177.*sqrt(18.),	// v2 wire 177
  CH_OHIPS_POSITION_V2+178.*sqrt(18.),	// v2 wire 178
  CH_OHIPS_POSITION_V2+179.*sqrt(18.),	// v2 wire 179
  CH_OHIPS_POSITION_V2+180.*sqrt(18.),	// v2 wire 180
  CH_OHIPS_POSITION_V2+181.*sqrt(18.),	// v2 wire 181
  CH_OHIPS_POSITION_V2+182.*sqrt(18.),	// v2 wire 182
  CH_OHIPS_POSITION_V2+183.*sqrt(18.),	// v2 wire 183
  CH_OHIPS_POSITION_V2+184.*sqrt(18.),	// v2 wire 184
  CH_OHIPS_POSITION_V2+185.*sqrt(18.),	// v2 wire 185
  CH_OHIPS_POSITION_V2+186.*sqrt(18.),	// v2 wire 186
  CH_OHIPS_POSITION_V2+187.*sqrt(18.),	// v2 wire 187
  CH_OHIPS_POSITION_V2+188.*sqrt(18.),	// v2 wire 188
  CH_OHIPS_POSITION_V2+189.*sqrt(18.),	// v2 wire 189
  CH_OHIPS_POSITION_V2+190.*sqrt(18.),	// v2 wire 190
  CH_OHIPS_POSITION_V2+191.*sqrt(18.),	// v2 wire 191
  CH_OHIPS_POSITION_V2+192.*sqrt(18.),	// v2 wire 192
  CH_OHIPS_POSITION_V2+193.*sqrt(18.),	// v2 wire 193
  CH_OHIPS_POSITION_V2+194.*sqrt(18.),	// v2 wire 194
  CH_OHIPS_POSITION_V2+195.*sqrt(18.),	// v2 wire 195
  CH_OHIPS_POSITION_V2+196.*sqrt(18.),	// v2 wire 196
  CH_OHIPS_POSITION_V2+197.*sqrt(18.),	// v2 wire 197
  CH_OHIPS_POSITION_V2+198.*sqrt(18.),	// v2 wire 198
  CH_OHIPS_POSITION_V2+199.*sqrt(18.),	// v2 wire 199
  CH_OHIPS_POSITION_V2+200.*sqrt(18.),	// v2 wire 200
  CH_OHIPS_POSITION_V2+201.*sqrt(18.),	// v2 wire 201
  CH_OHIPS_POSITION_V2+202.*sqrt(18.),	// v2 wire 202
  CH_OHIPS_POSITION_V2+203.*sqrt(18.),	// v2 wire 203
  CH_OHIPS_POSITION_V2+204.*sqrt(18.),	// v2 wire 204
  CH_OHIPS_POSITION_V2+205.*sqrt(18.),	// v2 wire 205
  CH_OHIPS_POSITION_V2+206.*sqrt(18.),	// v2 wire 206
  CH_OHIPS_POSITION_V2+207.*sqrt(18.),	// v2 wire 207
  CH_OHIPS_POSITION_V2+208.*sqrt(18.),	// v2 wire 208
  CH_OHIPS_POSITION_V2+209.*sqrt(18.),	// v2 wire 209
  CH_OHIPS_POSITION_V2+210.*sqrt(18.),	// v2 wire 210
  CH_OHIPS_POSITION_V2+211.*sqrt(18.),	// v2 wire 211
  CH_OHIPS_POSITION_V2+212.*sqrt(18.),	// v2 wire 212
  CH_OHIPS_POSITION_V2+213.*sqrt(18.),	// v2 wire 213
  CH_OHIPS_POSITION_V2+214.*sqrt(18.),	// v2 wire 214
  CH_OHIPS_POSITION_V2+215.*sqrt(18.),	// v2 wire 215
  CH_OHIPS_POSITION_V2+216.*sqrt(18.),	// v2 wire 216
  CH_OHIPS_POSITION_V2+217.*sqrt(18.),	// v2 wire 217
  CH_OHIPS_POSITION_V2+218.*sqrt(18.),	// v2 wire 218
  CH_OHIPS_POSITION_V2+219.*sqrt(18.),	// v2 wire 219
  CH_OHIPS_POSITION_V2+220.*sqrt(18.),	// v2 wire 220
  CH_OHIPS_POSITION_V2+221.*sqrt(18.),	// v2 wire 221
  CH_OHIPS_POSITION_V2+222.*sqrt(18.),	// v2 wire 222
  CH_OHIPS_POSITION_V2+223.*sqrt(18.),	// v2 wire 223
  CH_OHIPS_POSITION_V2+224.*sqrt(18.),	// v2 wire 224
  CH_OHIPS_POSITION_V2+225.*sqrt(18.),	// v2 wire 225
  CH_OHIPS_POSITION_V2+226.*sqrt(18.),	// v2 wire 226
  CH_OHIPS_POSITION_V2+227.*sqrt(18.),	// v2 wire 227
  CH_OHIPS_POSITION_V2+228.*sqrt(18.),	// v2 wire 228
  CH_OHIPS_POSITION_V2+229.*sqrt(18.),	// v2 wire 229
  CH_OHIPS_POSITION_V2+230.*sqrt(18.),	// v2 wire 230
  CH_OHIPS_POSITION_V2+231.*sqrt(18.),	// v2 wire 231
  CH_OHIPS_POSITION_V2+232.*sqrt(18.),	// v2 wire 232
  CH_OHIPS_POSITION_V2+233.*sqrt(18.),	// v2 wire 233
  CH_OHIPS_POSITION_V2+234.*sqrt(18.),	// v2 wire 234
  CH_OHIPS_POSITION_V2+235.*sqrt(18.),	// v2 wire 235
  CH_OHIPS_POSITION_V2+236.*sqrt(18.),	// v2 wire 236
  CH_OHIPS_POSITION_V2+237.*sqrt(18.),	// v2 wire 237
  CH_OHIPS_POSITION_V2+238.*sqrt(18.),	// v2 wire 238
  CH_OHIPS_POSITION_V2+239.*sqrt(18.),	// v2 wire 239
  CH_OHIPS_POSITION_V2+240.*sqrt(18.),	// v2 wire 240
  CH_OHIPS_POSITION_V2+241.*sqrt(18.),	// v2 wire 241
  CH_OHIPS_POSITION_V2+242.*sqrt(18.),	// v2 wire 242
  CH_OHIPS_POSITION_V2+243.*sqrt(18.),	// v2 wire 243
  CH_OHIPS_POSITION_V2+244.*sqrt(18.),	// v2 wire 244
  CH_OHIPS_POSITION_V2+245.*sqrt(18.),	// v2 wire 245
  CH_OHIPS_POSITION_V2+246.*sqrt(18.),	// v2 wire 246
  CH_OHIPS_POSITION_V2+247.*sqrt(18.),	// v2 wire 247
  CH_OHIPS_POSITION_V2+248.*sqrt(18.),	// v2 wire 248
  CH_OHIPS_POSITION_V2+249.*sqrt(18.),	// v2 wire 249
  CH_OHIPS_POSITION_V2+250.*sqrt(18.),	// v2 wire 250
  CH_OHIPS_POSITION_V2+251.*sqrt(18.),	// v2 wire 251
  CH_OHIPS_POSITION_V2+252.*sqrt(18.),	// v2 wire 252
  CH_OHIPS_POSITION_V2+253.*sqrt(18.),	// v2 wire 253
  CH_OHIPS_POSITION_V2+254.*sqrt(18.),	// v2 wire 254
  CH_OHIPS_POSITION_V2+255.*sqrt(18.),	// v2 wire 255
  CH_OHIPS_POSITION_V2+256.*sqrt(18.)	// v2 wire 256
};

vdcPosOffs vdcPosOffsOhips = {
  { position_ou1, offset_ou1, CH_OHIPS_MAX_U1 },
  { position_ov1, offset_ov1, CH_OHIPS_MAX_V1 },
  { position_ou2, offset_ou2, CH_OHIPS_MAX_U2 },
  { position_ov2, offset_ov2, CH_OHIPS_MAX_V2 }
};

#endif
