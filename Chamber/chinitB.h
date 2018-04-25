//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chinitB.h,v 2.9 1996-12-12 15:31:33 distler Exp $
//
// INIT file fuer die chamber++ library.
// Offset und Position fuer die Draehte
// von allen vier Ebenen der Driftkammern
// von Spektrometer B.
//

#ifndef __chinitB__
#define __chinitB__

const short CH_B_MAX_X1		= 368;
const short CH_B_MAX_S1		= 336;
const short CH_B_MAX_X2		= 400;
const short CH_B_MAX_S2		= 368;

const short CH_B_OFFSET_X1	= 629;
const short CH_B_OFFSET_S1	= 627;
const short CH_B_OFFSET_X2	= 628;
const short CH_B_OFFSET_S2	= 625;

const double CH_B_ANGLE		= 40.0;
const short  CH_B_FULLRANGE	= 1024;
const double CH_B_TIMETOLENGTH	= 0.5 * 0.05;

static short
offset_bx1[CH_B_MAX_X1] = {
  CH_B_OFFSET_X1,			// wire 1
  CH_B_OFFSET_X1,			// wire 2
  CH_B_OFFSET_X1,			// wire 3
  CH_B_OFFSET_X1,			// wire 4
  CH_B_OFFSET_X1,			// wire 5
  CH_B_OFFSET_X1,			// wire 6
  CH_B_OFFSET_X1,			// wire 7
  CH_B_OFFSET_X1,			// wire 8
  CH_B_OFFSET_X1,			// wire 9
  CH_B_OFFSET_X1,			// wire 10
  CH_B_OFFSET_X1,			// wire 11
  CH_B_OFFSET_X1,			// wire 12
  CH_B_OFFSET_X1,			// wire 13
  CH_B_OFFSET_X1,			// wire 14
  CH_B_OFFSET_X1,			// wire 15
  CH_B_OFFSET_X1,			// wire 16
  CH_B_OFFSET_X1,			// wire 17
  CH_B_OFFSET_X1,			// wire 18
  CH_B_OFFSET_X1,			// wire 19
  CH_B_OFFSET_X1,			// wire 20
  CH_B_OFFSET_X1,			// wire 21
  CH_B_OFFSET_X1,			// wire 22
  CH_B_OFFSET_X1,			// wire 23
  CH_B_OFFSET_X1,			// wire 24
  CH_B_OFFSET_X1,			// wire 25
  CH_B_OFFSET_X1,			// wire 26
  CH_B_OFFSET_X1,			// wire 27
  CH_B_OFFSET_X1,			// wire 28
  CH_B_OFFSET_X1,			// wire 29
  CH_B_OFFSET_X1,			// wire 30
  CH_B_OFFSET_X1,			// wire 31
  CH_B_OFFSET_X1,			// wire 32
  CH_B_OFFSET_X1,			// wire 33
  CH_B_OFFSET_X1,			// wire 34
  CH_B_OFFSET_X1,			// wire 35
  CH_B_OFFSET_X1,			// wire 36
  CH_B_OFFSET_X1,			// wire 37
  CH_B_OFFSET_X1,			// wire 38
  CH_B_OFFSET_X1,			// wire 39
  CH_B_OFFSET_X1,			// wire 40
  CH_B_OFFSET_X1,			// wire 41
  CH_B_OFFSET_X1,			// wire 42
  CH_B_OFFSET_X1,			// wire 43
  CH_B_OFFSET_X1,			// wire 44
  CH_B_OFFSET_X1,			// wire 45
  CH_B_OFFSET_X1,			// wire 46
  CH_B_OFFSET_X1,			// wire 47
  CH_B_OFFSET_X1,			// wire 48
  CH_B_OFFSET_X1,			// wire 49
  CH_B_OFFSET_X1,			// wire 50
  CH_B_OFFSET_X1,			// wire 51
  CH_B_OFFSET_X1,			// wire 52
  CH_B_OFFSET_X1,			// wire 53
  CH_B_OFFSET_X1,			// wire 54
  CH_B_OFFSET_X1,			// wire 55
  CH_B_OFFSET_X1,			// wire 56
  CH_B_OFFSET_X1,			// wire 57
  CH_B_OFFSET_X1,			// wire 58
  CH_B_OFFSET_X1,			// wire 59
  CH_B_OFFSET_X1,			// wire 60
  CH_B_OFFSET_X1,			// wire 61
  CH_B_OFFSET_X1,			// wire 62
  CH_B_OFFSET_X1,			// wire 63
  CH_B_OFFSET_X1,			// wire 64
  CH_B_OFFSET_X1,			// wire 65
  CH_B_OFFSET_X1,			// wire 66
  CH_B_OFFSET_X1,			// wire 67
  CH_B_OFFSET_X1,			// wire 68
  CH_B_OFFSET_X1,			// wire 69
  CH_B_OFFSET_X1,			// wire 70
  CH_B_OFFSET_X1,			// wire 71
  CH_B_OFFSET_X1,			// wire 72
  CH_B_OFFSET_X1,			// wire 73
  CH_B_OFFSET_X1,			// wire 74
  CH_B_OFFSET_X1,			// wire 75
  CH_B_OFFSET_X1,			// wire 76
  CH_B_OFFSET_X1,			// wire 77
  CH_B_OFFSET_X1,			// wire 78
  CH_B_OFFSET_X1,			// wire 79
  CH_B_OFFSET_X1,			// wire 80
  CH_B_OFFSET_X1,			// wire 81
  CH_B_OFFSET_X1,			// wire 82
  CH_B_OFFSET_X1,			// wire 83
  CH_B_OFFSET_X1,			// wire 84
  CH_B_OFFSET_X1,			// wire 85
  CH_B_OFFSET_X1,			// wire 86
  CH_B_OFFSET_X1,			// wire 87
  CH_B_OFFSET_X1,			// wire 88
  CH_B_OFFSET_X1,			// wire 89
  CH_B_OFFSET_X1,			// wire 90
  CH_B_OFFSET_X1,			// wire 91
  CH_B_OFFSET_X1,			// wire 92
  CH_B_OFFSET_X1,			// wire 93
  CH_B_OFFSET_X1,			// wire 94
  CH_B_OFFSET_X1,			// wire 95
  CH_B_OFFSET_X1,			// wire 96
  CH_B_OFFSET_X1,			// wire 97
  CH_B_OFFSET_X1,			// wire 98
  CH_B_OFFSET_X1,			// wire 99
  CH_B_OFFSET_X1,			// wire 100
  CH_B_OFFSET_X1,			// wire 101
  CH_B_OFFSET_X1,			// wire 102
  CH_B_OFFSET_X1,			// wire 103
  CH_B_OFFSET_X1,			// wire 104
  CH_B_OFFSET_X1,			// wire 105
  CH_B_OFFSET_X1,			// wire 106
  CH_B_OFFSET_X1,			// wire 107
  CH_B_OFFSET_X1,			// wire 108
  CH_B_OFFSET_X1,			// wire 109
  CH_B_OFFSET_X1,			// wire 110
  CH_B_OFFSET_X1,			// wire 111
  CH_B_OFFSET_X1,			// wire 112
  CH_B_OFFSET_X1,			// wire 113
  CH_B_OFFSET_X1,			// wire 114
  CH_B_OFFSET_X1,			// wire 115
  CH_B_OFFSET_X1,			// wire 116
  CH_B_OFFSET_X1,			// wire 117
  CH_B_OFFSET_X1,			// wire 118
  CH_B_OFFSET_X1,			// wire 119
  CH_B_OFFSET_X1,			// wire 120
  CH_B_OFFSET_X1,			// wire 121
  CH_B_OFFSET_X1,			// wire 122
  CH_B_OFFSET_X1,			// wire 123
  CH_B_OFFSET_X1,			// wire 124
  CH_B_OFFSET_X1,			// wire 125
  CH_B_OFFSET_X1,			// wire 126
  CH_B_OFFSET_X1,			// wire 127
  CH_B_OFFSET_X1,			// wire 128
  CH_B_OFFSET_X1,			// wire 129
  CH_B_OFFSET_X1,			// wire 130
  CH_B_OFFSET_X1,			// wire 131
  CH_B_OFFSET_X1,			// wire 132
  CH_B_OFFSET_X1,			// wire 133
  CH_B_OFFSET_X1,			// wire 134
  CH_B_OFFSET_X1,			// wire 135
  CH_B_OFFSET_X1,			// wire 136
  CH_B_OFFSET_X1,			// wire 137
  CH_B_OFFSET_X1,			// wire 138
  CH_B_OFFSET_X1,			// wire 139
  CH_B_OFFSET_X1,			// wire 140
  CH_B_OFFSET_X1,			// wire 141
  CH_B_OFFSET_X1,			// wire 142
  CH_B_OFFSET_X1,			// wire 143
  CH_B_OFFSET_X1,			// wire 144
  CH_B_OFFSET_X1,			// wire 145
  CH_B_OFFSET_X1,			// wire 146
  CH_B_OFFSET_X1,			// wire 147
  CH_B_OFFSET_X1,			// wire 148
  CH_B_OFFSET_X1,			// wire 149
  CH_B_OFFSET_X1,			// wire 150
  CH_B_OFFSET_X1,			// wire 151
  CH_B_OFFSET_X1,			// wire 152
  CH_B_OFFSET_X1,			// wire 153
  CH_B_OFFSET_X1,			// wire 154
  CH_B_OFFSET_X1,			// wire 155
  CH_B_OFFSET_X1,			// wire 156
  CH_B_OFFSET_X1,			// wire 157
  CH_B_OFFSET_X1,			// wire 158
  CH_B_OFFSET_X1,			// wire 159
  CH_B_OFFSET_X1,			// wire 160
  CH_B_OFFSET_X1,			// wire 161
  CH_B_OFFSET_X1,			// wire 162
  CH_B_OFFSET_X1,			// wire 163
  CH_B_OFFSET_X1,			// wire 164
  CH_B_OFFSET_X1,			// wire 165
  CH_B_OFFSET_X1,			// wire 166
  CH_B_OFFSET_X1,			// wire 167
  CH_B_OFFSET_X1,			// wire 168
  CH_B_OFFSET_X1,			// wire 169
  CH_B_OFFSET_X1,			// wire 170
  CH_B_OFFSET_X1,			// wire 171
  CH_B_OFFSET_X1,			// wire 172
  CH_B_OFFSET_X1,			// wire 173
  CH_B_OFFSET_X1,			// wire 174
  CH_B_OFFSET_X1,			// wire 175
  CH_B_OFFSET_X1,			// wire 176
  CH_B_OFFSET_X1,			// wire 177
  CH_B_OFFSET_X1,			// wire 178
  CH_B_OFFSET_X1,			// wire 179
  CH_B_OFFSET_X1,			// wire 180
  CH_B_OFFSET_X1,			// wire 181
  CH_B_OFFSET_X1,			// wire 182
  CH_B_OFFSET_X1,			// wire 183
  CH_B_OFFSET_X1,			// wire 184
  CH_B_OFFSET_X1,			// wire 185
  CH_B_OFFSET_X1,			// wire 186
  CH_B_OFFSET_X1,			// wire 187
  CH_B_OFFSET_X1,			// wire 188
  CH_B_OFFSET_X1,			// wire 189
  CH_B_OFFSET_X1,			// wire 190
  CH_B_OFFSET_X1,			// wire 191
  CH_B_OFFSET_X1,			// wire 192
  CH_B_OFFSET_X1,			// wire 193
  CH_B_OFFSET_X1,			// wire 194
  CH_B_OFFSET_X1,			// wire 195
  CH_B_OFFSET_X1,			// wire 196
  CH_B_OFFSET_X1,			// wire 197
  CH_B_OFFSET_X1,			// wire 198
  CH_B_OFFSET_X1,			// wire 199
  CH_B_OFFSET_X1,			// wire 200
  CH_B_OFFSET_X1,			// wire 201
  CH_B_OFFSET_X1,			// wire 202
  CH_B_OFFSET_X1,			// wire 203
  CH_B_OFFSET_X1,			// wire 204
  CH_B_OFFSET_X1,			// wire 205
  CH_B_OFFSET_X1,			// wire 206
  CH_B_OFFSET_X1,			// wire 207
  CH_B_OFFSET_X1,			// wire 208
  CH_B_OFFSET_X1,			// wire 209
  CH_B_OFFSET_X1,			// wire 210
  CH_B_OFFSET_X1,			// wire 211
  CH_B_OFFSET_X1,			// wire 212
  CH_B_OFFSET_X1,			// wire 213
  CH_B_OFFSET_X1,			// wire 214
  CH_B_OFFSET_X1,			// wire 215
  CH_B_OFFSET_X1,			// wire 216
  CH_B_OFFSET_X1,			// wire 217
  CH_B_OFFSET_X1,			// wire 218
  CH_B_OFFSET_X1,			// wire 219
  CH_B_OFFSET_X1,			// wire 220
  CH_B_OFFSET_X1,			// wire 221
  CH_B_OFFSET_X1,			// wire 222
  CH_B_OFFSET_X1,			// wire 223
  CH_B_OFFSET_X1,			// wire 224
  CH_B_OFFSET_X1,			// wire 225
  CH_B_OFFSET_X1,			// wire 226
  CH_B_OFFSET_X1,			// wire 227
  CH_B_OFFSET_X1,			// wire 228
  CH_B_OFFSET_X1,			// wire 229
  CH_B_OFFSET_X1,			// wire 230
  CH_B_OFFSET_X1,			// wire 231
  CH_B_OFFSET_X1,			// wire 232
  CH_B_OFFSET_X1,			// wire 233
  CH_B_OFFSET_X1,			// wire 234
  CH_B_OFFSET_X1,			// wire 235
  CH_B_OFFSET_X1,			// wire 236
  CH_B_OFFSET_X1,			// wire 237
  CH_B_OFFSET_X1,			// wire 238
  CH_B_OFFSET_X1,			// wire 239
  CH_B_OFFSET_X1,			// wire 240
  CH_B_OFFSET_X1,			// wire 241
  CH_B_OFFSET_X1,			// wire 242
  CH_B_OFFSET_X1,			// wire 243
  CH_B_OFFSET_X1,			// wire 244
  CH_B_OFFSET_X1,			// wire 245
  CH_B_OFFSET_X1,			// wire 246
  CH_B_OFFSET_X1,			// wire 247
  CH_B_OFFSET_X1,			// wire 248
  CH_B_OFFSET_X1,			// wire 249
  CH_B_OFFSET_X1,			// wire 250
  CH_B_OFFSET_X1,			// wire 251
  CH_B_OFFSET_X1,			// wire 252
  CH_B_OFFSET_X1,			// wire 253
  CH_B_OFFSET_X1,			// wire 254
  CH_B_OFFSET_X1,			// wire 255
  CH_B_OFFSET_X1,			// wire 256
  CH_B_OFFSET_X1,			// wire 257
  CH_B_OFFSET_X1,			// wire 258
  CH_B_OFFSET_X1,			// wire 259
  CH_B_OFFSET_X1,			// wire 260
  CH_B_OFFSET_X1,			// wire 261
  CH_B_OFFSET_X1,			// wire 262
  CH_B_OFFSET_X1,			// wire 263
  CH_B_OFFSET_X1,			// wire 264
  CH_B_OFFSET_X1,			// wire 265
  CH_B_OFFSET_X1,			// wire 266
  CH_B_OFFSET_X1,			// wire 267
  CH_B_OFFSET_X1,			// wire 268
  CH_B_OFFSET_X1,			// wire 269
  CH_B_OFFSET_X1,			// wire 270
  CH_B_OFFSET_X1,			// wire 271
  CH_B_OFFSET_X1,			// wire 272
  CH_B_OFFSET_X1,			// wire 273
  CH_B_OFFSET_X1,			// wire 274
  CH_B_OFFSET_X1,			// wire 275
  CH_B_OFFSET_X1,			// wire 276
  CH_B_OFFSET_X1,			// wire 277
  CH_B_OFFSET_X1,			// wire 278
  CH_B_OFFSET_X1,			// wire 279
  CH_B_OFFSET_X1,			// wire 280
  CH_B_OFFSET_X1,			// wire 281
  CH_B_OFFSET_X1,			// wire 282
  CH_B_OFFSET_X1,			// wire 283
  CH_B_OFFSET_X1,			// wire 284
  CH_B_OFFSET_X1,			// wire 285
  CH_B_OFFSET_X1,			// wire 286
  CH_B_OFFSET_X1,			// wire 287
  CH_B_OFFSET_X1,			// wire 288
  CH_B_OFFSET_X1,			// wire 289
  CH_B_OFFSET_X1,			// wire 290
  CH_B_OFFSET_X1,			// wire 291
  CH_B_OFFSET_X1,			// wire 292
  CH_B_OFFSET_X1,			// wire 293
  CH_B_OFFSET_X1,			// wire 294
  CH_B_OFFSET_X1,			// wire 295
  CH_B_OFFSET_X1,			// wire 296
  CH_B_OFFSET_X1,			// wire 297
  CH_B_OFFSET_X1,			// wire 298
  CH_B_OFFSET_X1,			// wire 299
  CH_B_OFFSET_X1,			// wire 300
  CH_B_OFFSET_X1,			// wire 301
  CH_B_OFFSET_X1,			// wire 302
  CH_B_OFFSET_X1,			// wire 303
  CH_B_OFFSET_X1,			// wire 304
  CH_B_OFFSET_X1,			// wire 305
  CH_B_OFFSET_X1,			// wire 306
  CH_B_OFFSET_X1,			// wire 307
  CH_B_OFFSET_X1,			// wire 308
  CH_B_OFFSET_X1,			// wire 309
  CH_B_OFFSET_X1,			// wire 310
  CH_B_OFFSET_X1,			// wire 311
  CH_B_OFFSET_X1,			// wire 312
  CH_B_OFFSET_X1,			// wire 313
  CH_B_OFFSET_X1,			// wire 314
  CH_B_OFFSET_X1,			// wire 315
  CH_B_OFFSET_X1,			// wire 316
  CH_B_OFFSET_X1,			// wire 317
  CH_B_OFFSET_X1,			// wire 318
  CH_B_OFFSET_X1,			// wire 319
  CH_B_OFFSET_X1,			// wire 320
  CH_B_OFFSET_X1,			// wire 321
  CH_B_OFFSET_X1,			// wire 322
  CH_B_OFFSET_X1,			// wire 323
  CH_B_OFFSET_X1,			// wire 324
  CH_B_OFFSET_X1,			// wire 325
  CH_B_OFFSET_X1,			// wire 326
  CH_B_OFFSET_X1,			// wire 327
  CH_B_OFFSET_X1,			// wire 328
  CH_B_OFFSET_X1,			// wire 329
  CH_B_OFFSET_X1,			// wire 330
  CH_B_OFFSET_X1,			// wire 331
  CH_B_OFFSET_X1,			// wire 332
  CH_B_OFFSET_X1,			// wire 333
  CH_B_OFFSET_X1,			// wire 334
  CH_B_OFFSET_X1,			// wire 335
  CH_B_OFFSET_X1,			// wire 336
  CH_B_OFFSET_X1,			// wire 337
  CH_B_OFFSET_X1,			// wire 338
  CH_B_OFFSET_X1,			// wire 339
  CH_B_OFFSET_X1,			// wire 340
  CH_B_OFFSET_X1,			// wire 341
  CH_B_OFFSET_X1,			// wire 342
  CH_B_OFFSET_X1,			// wire 343
  CH_B_OFFSET_X1,			// wire 344
  CH_B_OFFSET_X1,			// wire 345
  CH_B_OFFSET_X1,			// wire 346
  CH_B_OFFSET_X1,			// wire 347
  CH_B_OFFSET_X1,			// wire 348
  CH_B_OFFSET_X1,			// wire 349
  CH_B_OFFSET_X1,			// wire 350
  CH_B_OFFSET_X1,			// wire 351
  CH_B_OFFSET_X1,			// wire 352
  CH_B_OFFSET_X1,			// wire 353
  CH_B_OFFSET_X1,			// wire 354
  CH_B_OFFSET_X1,			// wire 355
  CH_B_OFFSET_X1,			// wire 356
  CH_B_OFFSET_X1,			// wire 357
  CH_B_OFFSET_X1,			// wire 358
  CH_B_OFFSET_X1,			// wire 359
  CH_B_OFFSET_X1,			// wire 360
  CH_B_OFFSET_X1,			// wire 361
  CH_B_OFFSET_X1,			// wire 362
  CH_B_OFFSET_X1,			// wire 363
  CH_B_OFFSET_X1,			// wire 364
  CH_B_OFFSET_X1,			// wire 365
  CH_B_OFFSET_X1,			// wire 366
  CH_B_OFFSET_X1,			// wire 367
  CH_B_OFFSET_X1			// wire 368
};

static short
offset_bs1[CH_B_MAX_S1] = {
  CH_B_OFFSET_S1,			// wire 1
  CH_B_OFFSET_S1,			// wire 2
  CH_B_OFFSET_S1,			// wire 3
  CH_B_OFFSET_S1,			// wire 4
  CH_B_OFFSET_S1,			// wire 5
  CH_B_OFFSET_S1,			// wire 6
  CH_B_OFFSET_S1,			// wire 7
  CH_B_OFFSET_S1,			// wire 8
  CH_B_OFFSET_S1,			// wire 9
  CH_B_OFFSET_S1,			// wire 10
  CH_B_OFFSET_S1,			// wire 11
  CH_B_OFFSET_S1,			// wire 12
  CH_B_OFFSET_S1,			// wire 13
  CH_B_OFFSET_S1,			// wire 14
  CH_B_OFFSET_S1,			// wire 15
  CH_B_OFFSET_S1,			// wire 16
  CH_B_OFFSET_S1,			// wire 17
  CH_B_OFFSET_S1,			// wire 18
  CH_B_OFFSET_S1,			// wire 19
  CH_B_OFFSET_S1,			// wire 20
  CH_B_OFFSET_S1,			// wire 21
  CH_B_OFFSET_S1,			// wire 22
  CH_B_OFFSET_S1,			// wire 23
  CH_B_OFFSET_S1,			// wire 24
  CH_B_OFFSET_S1,			// wire 25
  CH_B_OFFSET_S1,			// wire 26
  CH_B_OFFSET_S1,			// wire 27
  CH_B_OFFSET_S1,			// wire 28
  CH_B_OFFSET_S1,			// wire 29
  CH_B_OFFSET_S1,			// wire 30
  CH_B_OFFSET_S1,			// wire 31
  CH_B_OFFSET_S1,			// wire 32
  CH_B_OFFSET_S1,			// wire 33
  CH_B_OFFSET_S1,			// wire 34
  CH_B_OFFSET_S1,			// wire 35
  CH_B_OFFSET_S1,			// wire 36
  CH_B_OFFSET_S1,			// wire 37
  CH_B_OFFSET_S1,			// wire 38
  CH_B_OFFSET_S1,			// wire 39
  CH_B_OFFSET_S1,			// wire 40
  CH_B_OFFSET_S1,			// wire 41
  CH_B_OFFSET_S1,			// wire 42
  CH_B_OFFSET_S1,			// wire 43
  CH_B_OFFSET_S1,			// wire 44
  CH_B_OFFSET_S1,			// wire 45
  CH_B_OFFSET_S1,			// wire 46
  CH_B_OFFSET_S1,			// wire 47
  CH_B_OFFSET_S1,			// wire 48
  CH_B_OFFSET_S1,			// wire 49
  CH_B_OFFSET_S1,			// wire 50
  CH_B_OFFSET_S1,			// wire 51
  CH_B_OFFSET_S1,			// wire 52
  CH_B_OFFSET_S1,			// wire 53
  CH_B_OFFSET_S1,			// wire 54
  CH_B_OFFSET_S1,			// wire 55
  CH_B_OFFSET_S1,			// wire 56
  CH_B_OFFSET_S1,			// wire 57
  CH_B_OFFSET_S1,			// wire 58
  CH_B_OFFSET_S1,			// wire 59
  CH_B_OFFSET_S1,			// wire 60
  CH_B_OFFSET_S1,			// wire 61
  CH_B_OFFSET_S1,			// wire 62
  CH_B_OFFSET_S1,			// wire 63
  CH_B_OFFSET_S1,			// wire 64
  CH_B_OFFSET_S1,			// wire 65
  CH_B_OFFSET_S1,			// wire 66
  CH_B_OFFSET_S1,			// wire 67
  CH_B_OFFSET_S1,			// wire 68
  CH_B_OFFSET_S1,			// wire 69
  CH_B_OFFSET_S1,			// wire 70
  CH_B_OFFSET_S1,			// wire 71
  CH_B_OFFSET_S1,			// wire 72
  CH_B_OFFSET_S1,			// wire 73
  CH_B_OFFSET_S1,			// wire 74
  CH_B_OFFSET_S1,			// wire 75
  CH_B_OFFSET_S1,			// wire 76
  CH_B_OFFSET_S1,			// wire 77
  CH_B_OFFSET_S1,			// wire 78
  CH_B_OFFSET_S1,			// wire 79
  CH_B_OFFSET_S1,			// wire 80
  CH_B_OFFSET_S1,			// wire 81
  CH_B_OFFSET_S1,			// wire 82
  CH_B_OFFSET_S1,			// wire 83
  CH_B_OFFSET_S1,			// wire 84
  CH_B_OFFSET_S1,			// wire 85
  CH_B_OFFSET_S1,			// wire 86
  CH_B_OFFSET_S1,			// wire 87
  CH_B_OFFSET_S1,			// wire 88
  CH_B_OFFSET_S1,			// wire 89
  CH_B_OFFSET_S1,			// wire 90
  CH_B_OFFSET_S1,			// wire 91
  CH_B_OFFSET_S1,			// wire 92
  CH_B_OFFSET_S1,			// wire 93
  CH_B_OFFSET_S1,			// wire 94
  CH_B_OFFSET_S1,			// wire 95
  CH_B_OFFSET_S1,			// wire 96
  CH_B_OFFSET_S1,			// wire 97
  CH_B_OFFSET_S1,			// wire 98
  CH_B_OFFSET_S1,			// wire 99
  CH_B_OFFSET_S1,			// wire 100
  CH_B_OFFSET_S1,			// wire 101
  CH_B_OFFSET_S1,			// wire 102
  CH_B_OFFSET_S1,			// wire 103
  CH_B_OFFSET_S1,			// wire 104
  CH_B_OFFSET_S1,			// wire 105
  CH_B_OFFSET_S1,			// wire 106
  CH_B_OFFSET_S1,			// wire 107
  CH_B_OFFSET_S1,			// wire 108
  CH_B_OFFSET_S1,			// wire 109
  CH_B_OFFSET_S1,			// wire 110
  CH_B_OFFSET_S1,			// wire 111
  CH_B_OFFSET_S1,			// wire 112
  CH_B_OFFSET_S1,			// wire 113
  CH_B_OFFSET_S1,			// wire 114
  CH_B_OFFSET_S1,			// wire 115
  CH_B_OFFSET_S1,			// wire 116
  CH_B_OFFSET_S1,			// wire 117
  CH_B_OFFSET_S1,			// wire 118
  CH_B_OFFSET_S1,			// wire 119
  CH_B_OFFSET_S1,			// wire 120
  CH_B_OFFSET_S1,			// wire 121
  CH_B_OFFSET_S1,			// wire 122
  CH_B_OFFSET_S1,			// wire 123
  CH_B_OFFSET_S1,			// wire 124
  CH_B_OFFSET_S1,			// wire 125
  CH_B_OFFSET_S1,			// wire 126
  CH_B_OFFSET_S1,			// wire 127
  CH_B_OFFSET_S1,			// wire 128
  CH_B_OFFSET_S1,			// wire 129
  CH_B_OFFSET_S1,			// wire 130
  CH_B_OFFSET_S1,			// wire 131
  CH_B_OFFSET_S1,			// wire 132
  CH_B_OFFSET_S1,			// wire 133
  CH_B_OFFSET_S1,			// wire 134
  CH_B_OFFSET_S1,			// wire 135
  CH_B_OFFSET_S1,			// wire 136
  CH_B_OFFSET_S1,			// wire 137
  CH_B_OFFSET_S1,			// wire 138
  CH_B_OFFSET_S1,			// wire 139
  CH_B_OFFSET_S1,			// wire 140
  CH_B_OFFSET_S1,			// wire 141
  CH_B_OFFSET_S1,			// wire 142
  CH_B_OFFSET_S1,			// wire 143
  CH_B_OFFSET_S1,			// wire 144
  CH_B_OFFSET_S1,			// wire 145
  CH_B_OFFSET_S1,			// wire 146
  CH_B_OFFSET_S1,			// wire 147
  CH_B_OFFSET_S1,			// wire 148
  CH_B_OFFSET_S1,			// wire 149
  CH_B_OFFSET_S1,			// wire 150
  CH_B_OFFSET_S1,			// wire 151
  CH_B_OFFSET_S1,			// wire 152
  CH_B_OFFSET_S1,			// wire 153
  CH_B_OFFSET_S1,			// wire 154
  CH_B_OFFSET_S1,			// wire 155
  CH_B_OFFSET_S1,			// wire 156
  CH_B_OFFSET_S1,			// wire 157
  CH_B_OFFSET_S1,			// wire 158
  CH_B_OFFSET_S1,			// wire 159
  CH_B_OFFSET_S1,			// wire 160
  CH_B_OFFSET_S1,			// wire 161
  CH_B_OFFSET_S1,			// wire 162
  CH_B_OFFSET_S1,			// wire 163
  CH_B_OFFSET_S1,			// wire 164
  CH_B_OFFSET_S1,			// wire 165
  CH_B_OFFSET_S1,			// wire 166
  CH_B_OFFSET_S1,			// wire 167
  CH_B_OFFSET_S1,			// wire 168
  CH_B_OFFSET_S1,			// wire 169
  CH_B_OFFSET_S1,			// wire 170
  CH_B_OFFSET_S1,			// wire 171
  CH_B_OFFSET_S1,			// wire 172
  CH_B_OFFSET_S1,			// wire 173
  CH_B_OFFSET_S1,			// wire 174
  CH_B_OFFSET_S1,			// wire 175
  CH_B_OFFSET_S1,			// wire 176
  CH_B_OFFSET_S1,			// wire 177
  CH_B_OFFSET_S1,			// wire 178
  CH_B_OFFSET_S1,			// wire 179
  CH_B_OFFSET_S1,			// wire 180
  CH_B_OFFSET_S1,			// wire 181
  CH_B_OFFSET_S1,			// wire 182
  CH_B_OFFSET_S1,			// wire 183
  CH_B_OFFSET_S1,			// wire 184
  CH_B_OFFSET_S1,			// wire 185
  CH_B_OFFSET_S1,			// wire 186
  CH_B_OFFSET_S1,			// wire 187
  CH_B_OFFSET_S1,			// wire 188
  CH_B_OFFSET_S1,			// wire 189
  CH_B_OFFSET_S1,			// wire 190
  CH_B_OFFSET_S1,			// wire 191
  CH_B_OFFSET_S1,			// wire 192
  CH_B_OFFSET_S1,			// wire 193
  CH_B_OFFSET_S1,			// wire 194
  CH_B_OFFSET_S1,			// wire 195
  CH_B_OFFSET_S1,			// wire 196
  CH_B_OFFSET_S1,			// wire 197
  CH_B_OFFSET_S1,			// wire 198
  CH_B_OFFSET_S1,			// wire 199
  CH_B_OFFSET_S1,			// wire 200
  CH_B_OFFSET_S1,			// wire 201
  CH_B_OFFSET_S1,			// wire 202
  CH_B_OFFSET_S1,			// wire 203
  CH_B_OFFSET_S1,			// wire 204
  CH_B_OFFSET_S1,			// wire 205
  CH_B_OFFSET_S1,			// wire 206
  CH_B_OFFSET_S1,			// wire 207
  CH_B_OFFSET_S1,			// wire 208
  CH_B_OFFSET_S1,			// wire 209
  CH_B_OFFSET_S1,			// wire 210
  CH_B_OFFSET_S1,			// wire 211
  CH_B_OFFSET_S1,			// wire 212
  CH_B_OFFSET_S1,			// wire 213
  CH_B_OFFSET_S1,			// wire 214
  CH_B_OFFSET_S1,			// wire 215
  CH_B_OFFSET_S1,			// wire 216
  CH_B_OFFSET_S1,			// wire 217
  CH_B_OFFSET_S1,			// wire 218
  CH_B_OFFSET_S1,			// wire 219
  CH_B_OFFSET_S1,			// wire 220
  CH_B_OFFSET_S1,			// wire 221
  CH_B_OFFSET_S1,			// wire 222
  CH_B_OFFSET_S1,			// wire 223
  CH_B_OFFSET_S1,			// wire 224
  CH_B_OFFSET_S1,			// wire 225
  CH_B_OFFSET_S1,			// wire 226
  CH_B_OFFSET_S1,			// wire 227
  CH_B_OFFSET_S1,			// wire 228
  CH_B_OFFSET_S1,			// wire 229
  CH_B_OFFSET_S1,			// wire 230
  CH_B_OFFSET_S1,			// wire 231
  CH_B_OFFSET_S1,			// wire 232
  CH_B_OFFSET_S1,			// wire 233
  CH_B_OFFSET_S1,			// wire 234
  CH_B_OFFSET_S1,			// wire 235
  CH_B_OFFSET_S1,			// wire 236
  CH_B_OFFSET_S1,			// wire 237
  CH_B_OFFSET_S1,			// wire 238
  CH_B_OFFSET_S1,			// wire 239
  CH_B_OFFSET_S1,			// wire 240
  CH_B_OFFSET_S1,			// wire 241
  CH_B_OFFSET_S1,			// wire 242
  CH_B_OFFSET_S1,			// wire 243
  CH_B_OFFSET_S1,			// wire 244
  CH_B_OFFSET_S1,			// wire 245
  CH_B_OFFSET_S1,			// wire 246
  CH_B_OFFSET_S1,			// wire 247
  CH_B_OFFSET_S1,			// wire 248
  CH_B_OFFSET_S1,			// wire 249
  CH_B_OFFSET_S1,			// wire 250
  CH_B_OFFSET_S1,			// wire 251
  CH_B_OFFSET_S1,			// wire 252
  CH_B_OFFSET_S1,			// wire 253
  CH_B_OFFSET_S1,			// wire 254
  CH_B_OFFSET_S1,			// wire 255
  CH_B_OFFSET_S1,			// wire 256
  CH_B_OFFSET_S1,			// wire 257
  CH_B_OFFSET_S1,			// wire 258
  CH_B_OFFSET_S1,			// wire 259
  CH_B_OFFSET_S1,			// wire 260
  CH_B_OFFSET_S1,			// wire 261
  CH_B_OFFSET_S1,			// wire 262
  CH_B_OFFSET_S1,			// wire 263
  CH_B_OFFSET_S1,			// wire 264
  CH_B_OFFSET_S1,			// wire 265
  CH_B_OFFSET_S1,			// wire 266
  CH_B_OFFSET_S1,			// wire 267
  CH_B_OFFSET_S1,			// wire 268
  CH_B_OFFSET_S1,			// wire 269
  CH_B_OFFSET_S1,			// wire 270
  CH_B_OFFSET_S1,			// wire 271
  CH_B_OFFSET_S1,			// wire 272
  CH_B_OFFSET_S1,			// wire 273
  CH_B_OFFSET_S1,			// wire 274
  CH_B_OFFSET_S1,			// wire 275
  CH_B_OFFSET_S1,			// wire 276
  CH_B_OFFSET_S1,			// wire 277
  CH_B_OFFSET_S1,			// wire 278
  CH_B_OFFSET_S1,			// wire 279
  CH_B_OFFSET_S1,			// wire 280
  CH_B_OFFSET_S1,			// wire 281
  CH_B_OFFSET_S1,			// wire 282
  CH_B_OFFSET_S1,			// wire 283
  CH_B_OFFSET_S1,			// wire 284
  CH_B_OFFSET_S1,			// wire 285
  CH_B_OFFSET_S1,			// wire 286
  CH_B_OFFSET_S1,			// wire 287
  CH_B_OFFSET_S1,			// wire 288
  CH_B_OFFSET_S1,			// wire 289
  CH_B_OFFSET_S1,			// wire 290
  CH_B_OFFSET_S1,			// wire 291
  CH_B_OFFSET_S1,			// wire 292
  CH_B_OFFSET_S1,			// wire 293
  CH_B_OFFSET_S1,			// wire 294
  CH_B_OFFSET_S1,			// wire 295
  CH_B_OFFSET_S1,			// wire 296
  CH_B_OFFSET_S1,			// wire 297
  CH_B_OFFSET_S1,			// wire 298
  CH_B_OFFSET_S1,			// wire 299
  CH_B_OFFSET_S1,			// wire 300
  CH_B_OFFSET_S1,			// wire 301
  CH_B_OFFSET_S1,			// wire 302
  CH_B_OFFSET_S1,			// wire 303
  CH_B_OFFSET_S1,			// wire 304
  CH_B_OFFSET_S1,			// wire 305
  CH_B_OFFSET_S1,			// wire 306
  CH_B_OFFSET_S1,			// wire 307
  CH_B_OFFSET_S1,			// wire 308
  CH_B_OFFSET_S1,			// wire 309
  CH_B_OFFSET_S1,			// wire 310
  CH_B_OFFSET_S1,			// wire 311
  CH_B_OFFSET_S1,			// wire 312
  CH_B_OFFSET_S1,			// wire 313
  CH_B_OFFSET_S1,			// wire 314
  CH_B_OFFSET_S1,			// wire 315
  CH_B_OFFSET_S1,			// wire 316
  CH_B_OFFSET_S1,			// wire 317
  CH_B_OFFSET_S1,			// wire 318
  CH_B_OFFSET_S1,			// wire 319
  CH_B_OFFSET_S1,			// wire 320
  CH_B_OFFSET_S1,			// wire 321
  CH_B_OFFSET_S1,			// wire 322
  CH_B_OFFSET_S1,			// wire 323
  CH_B_OFFSET_S1,			// wire 324
  CH_B_OFFSET_S1,			// wire 325
  CH_B_OFFSET_S1,			// wire 326
  CH_B_OFFSET_S1,			// wire 327
  CH_B_OFFSET_S1,			// wire 328
  CH_B_OFFSET_S1,			// wire 329
  CH_B_OFFSET_S1,			// wire 330
  CH_B_OFFSET_S1,			// wire 331
  CH_B_OFFSET_S1,			// wire 332
  CH_B_OFFSET_S1,			// wire 333
  CH_B_OFFSET_S1,			// wire 334
  CH_B_OFFSET_S1,			// wire 335
  CH_B_OFFSET_S1			// wire 336
};

static short
offset_bx2[CH_B_MAX_X2] = {
  CH_B_OFFSET_X2,			// wire 1
  CH_B_OFFSET_X2,			// wire 2
  CH_B_OFFSET_X2,			// wire 3
  CH_B_OFFSET_X2,			// wire 4
  CH_B_OFFSET_X2,			// wire 5
  CH_B_OFFSET_X2,			// wire 6
  CH_B_OFFSET_X2,			// wire 7
  CH_B_OFFSET_X2,			// wire 8
  CH_B_OFFSET_X2,			// wire 9
  CH_B_OFFSET_X2,			// wire 10
  CH_B_OFFSET_X2,			// wire 11
  CH_B_OFFSET_X2,			// wire 12
  CH_B_OFFSET_X2,			// wire 13
  CH_B_OFFSET_X2,			// wire 14
  CH_B_OFFSET_X2,			// wire 15
  CH_B_OFFSET_X2,			// wire 16
  CH_B_OFFSET_X2,			// wire 17
  CH_B_OFFSET_X2,			// wire 18
  CH_B_OFFSET_X2,			// wire 19
  CH_B_OFFSET_X2,			// wire 20
  CH_B_OFFSET_X2,			// wire 21
  CH_B_OFFSET_X2,			// wire 22
  CH_B_OFFSET_X2,			// wire 23
  CH_B_OFFSET_X2,			// wire 24
  CH_B_OFFSET_X2,			// wire 25
  CH_B_OFFSET_X2,			// wire 26
  CH_B_OFFSET_X2,			// wire 27
  CH_B_OFFSET_X2,			// wire 28
  CH_B_OFFSET_X2,			// wire 29
  CH_B_OFFSET_X2,			// wire 30
  CH_B_OFFSET_X2,			// wire 31
  CH_B_OFFSET_X2,			// wire 32
  CH_B_OFFSET_X2,			// wire 33
  CH_B_OFFSET_X2,			// wire 34
  CH_B_OFFSET_X2,			// wire 35
  CH_B_OFFSET_X2,			// wire 36
  CH_B_OFFSET_X2,			// wire 37
  CH_B_OFFSET_X2,			// wire 38
  CH_B_OFFSET_X2,			// wire 39
  CH_B_OFFSET_X2,			// wire 40
  CH_B_OFFSET_X2,			// wire 41
  CH_B_OFFSET_X2,			// wire 42
  CH_B_OFFSET_X2,			// wire 43
  CH_B_OFFSET_X2,			// wire 44
  CH_B_OFFSET_X2,			// wire 45
  CH_B_OFFSET_X2,			// wire 46
  CH_B_OFFSET_X2,			// wire 47
  CH_B_OFFSET_X2,			// wire 48
  CH_B_OFFSET_X2,			// wire 49
  CH_B_OFFSET_X2,			// wire 50
  CH_B_OFFSET_X2,			// wire 51
  CH_B_OFFSET_X2,			// wire 52
  CH_B_OFFSET_X2,			// wire 53
  CH_B_OFFSET_X2,			// wire 54
  CH_B_OFFSET_X2,			// wire 55
  CH_B_OFFSET_X2,			// wire 56
  CH_B_OFFSET_X2,			// wire 57
  CH_B_OFFSET_X2,			// wire 58
  CH_B_OFFSET_X2,			// wire 59
  CH_B_OFFSET_X2,			// wire 60
  CH_B_OFFSET_X2,			// wire 61
  CH_B_OFFSET_X2,			// wire 62
  CH_B_OFFSET_X2,			// wire 63
  CH_B_OFFSET_X2,			// wire 64
  CH_B_OFFSET_X2,			// wire 65
  CH_B_OFFSET_X2,			// wire 66
  CH_B_OFFSET_X2,			// wire 67
  CH_B_OFFSET_X2,			// wire 68
  CH_B_OFFSET_X2,			// wire 69
  CH_B_OFFSET_X2,			// wire 70
  CH_B_OFFSET_X2,			// wire 71
  CH_B_OFFSET_X2,			// wire 72
  CH_B_OFFSET_X2,			// wire 73
  CH_B_OFFSET_X2,			// wire 74
  CH_B_OFFSET_X2,			// wire 75
  CH_B_OFFSET_X2,			// wire 76
  CH_B_OFFSET_X2,			// wire 77
  CH_B_OFFSET_X2,			// wire 78
  CH_B_OFFSET_X2,			// wire 79
  CH_B_OFFSET_X2,			// wire 80
  CH_B_OFFSET_X2,			// wire 81
  CH_B_OFFSET_X2,			// wire 82
  CH_B_OFFSET_X2,			// wire 83
  CH_B_OFFSET_X2,			// wire 84
  CH_B_OFFSET_X2,			// wire 85
  CH_B_OFFSET_X2,			// wire 86
  CH_B_OFFSET_X2,			// wire 87
  CH_B_OFFSET_X2,			// wire 88
  CH_B_OFFSET_X2,			// wire 89
  CH_B_OFFSET_X2,			// wire 90
  CH_B_OFFSET_X2,			// wire 91
  CH_B_OFFSET_X2,			// wire 92
  CH_B_OFFSET_X2,			// wire 93
  CH_B_OFFSET_X2,			// wire 94
  CH_B_OFFSET_X2,			// wire 95
  CH_B_OFFSET_X2,			// wire 96
  CH_B_OFFSET_X2,			// wire 97
  CH_B_OFFSET_X2,			// wire 98
  CH_B_OFFSET_X2,			// wire 99
  CH_B_OFFSET_X2,			// wire 100
  CH_B_OFFSET_X2,			// wire 101
  CH_B_OFFSET_X2,			// wire 102
  CH_B_OFFSET_X2,			// wire 103
  CH_B_OFFSET_X2,			// wire 104
  CH_B_OFFSET_X2,			// wire 105
  CH_B_OFFSET_X2,			// wire 106
  CH_B_OFFSET_X2,			// wire 107
  CH_B_OFFSET_X2,			// wire 108
  CH_B_OFFSET_X2,			// wire 109
  CH_B_OFFSET_X2,			// wire 110
  CH_B_OFFSET_X2,			// wire 111
  CH_B_OFFSET_X2,			// wire 112
  CH_B_OFFSET_X2,			// wire 113
  CH_B_OFFSET_X2,			// wire 114
  CH_B_OFFSET_X2,			// wire 115
  CH_B_OFFSET_X2,			// wire 116
  CH_B_OFFSET_X2,			// wire 117
  CH_B_OFFSET_X2,			// wire 118
  CH_B_OFFSET_X2,			// wire 119
  CH_B_OFFSET_X2,			// wire 120
  CH_B_OFFSET_X2,			// wire 121
  CH_B_OFFSET_X2,			// wire 122
  CH_B_OFFSET_X2,			// wire 123
  CH_B_OFFSET_X2,			// wire 124
  CH_B_OFFSET_X2,			// wire 125
  CH_B_OFFSET_X2,			// wire 126
  CH_B_OFFSET_X2,			// wire 127
  CH_B_OFFSET_X2,			// wire 128
  CH_B_OFFSET_X2,			// wire 129
  CH_B_OFFSET_X2,			// wire 130
  CH_B_OFFSET_X2,			// wire 131
  CH_B_OFFSET_X2,			// wire 132
  CH_B_OFFSET_X2,			// wire 133
  CH_B_OFFSET_X2,			// wire 134
  CH_B_OFFSET_X2,			// wire 135
  CH_B_OFFSET_X2,			// wire 136
  CH_B_OFFSET_X2,			// wire 137
  CH_B_OFFSET_X2,			// wire 138
  CH_B_OFFSET_X2,			// wire 139
  CH_B_OFFSET_X2,			// wire 140
  CH_B_OFFSET_X2,			// wire 141
  CH_B_OFFSET_X2,			// wire 142
  CH_B_OFFSET_X2,			// wire 143
  CH_B_OFFSET_X2,			// wire 144
  CH_B_OFFSET_X2,			// wire 145
  CH_B_OFFSET_X2,			// wire 146
  CH_B_OFFSET_X2,			// wire 147
  CH_B_OFFSET_X2,			// wire 148
  CH_B_OFFSET_X2,			// wire 149
  CH_B_OFFSET_X2,			// wire 150
  CH_B_OFFSET_X2,			// wire 151
  CH_B_OFFSET_X2,			// wire 152
  CH_B_OFFSET_X2,			// wire 153
  CH_B_OFFSET_X2,			// wire 154
  CH_B_OFFSET_X2,			// wire 155
  CH_B_OFFSET_X2,			// wire 156
  CH_B_OFFSET_X2,			// wire 157
  CH_B_OFFSET_X2,			// wire 158
  CH_B_OFFSET_X2,			// wire 159
  CH_B_OFFSET_X2,			// wire 160
  CH_B_OFFSET_X2,			// wire 161
  CH_B_OFFSET_X2,			// wire 162
  CH_B_OFFSET_X2,			// wire 163
  CH_B_OFFSET_X2,			// wire 164
  CH_B_OFFSET_X2,			// wire 165
  CH_B_OFFSET_X2,			// wire 166
  CH_B_OFFSET_X2,			// wire 167
  CH_B_OFFSET_X2,			// wire 168
  CH_B_OFFSET_X2,			// wire 169
  CH_B_OFFSET_X2,			// wire 170
  CH_B_OFFSET_X2,			// wire 171
  CH_B_OFFSET_X2,			// wire 172
  CH_B_OFFSET_X2,			// wire 173
  CH_B_OFFSET_X2,			// wire 174
  CH_B_OFFSET_X2,			// wire 175
  CH_B_OFFSET_X2,			// wire 176
  CH_B_OFFSET_X2,			// wire 177
  CH_B_OFFSET_X2,			// wire 178
  CH_B_OFFSET_X2,			// wire 179
  CH_B_OFFSET_X2,			// wire 180
  CH_B_OFFSET_X2,			// wire 181
  CH_B_OFFSET_X2,			// wire 182
  CH_B_OFFSET_X2,			// wire 183
  CH_B_OFFSET_X2,			// wire 184
  CH_B_OFFSET_X2,			// wire 185
  CH_B_OFFSET_X2,			// wire 186
  CH_B_OFFSET_X2,			// wire 187
  CH_B_OFFSET_X2,			// wire 188
  CH_B_OFFSET_X2,			// wire 189
  CH_B_OFFSET_X2,			// wire 190
  CH_B_OFFSET_X2,			// wire 191
  CH_B_OFFSET_X2,			// wire 192
  CH_B_OFFSET_X2,			// wire 193
  CH_B_OFFSET_X2,			// wire 194
  CH_B_OFFSET_X2,			// wire 195
  CH_B_OFFSET_X2,			// wire 196
  CH_B_OFFSET_X2,			// wire 197
  CH_B_OFFSET_X2,			// wire 198
  CH_B_OFFSET_X2,			// wire 199
  CH_B_OFFSET_X2,			// wire 200
  CH_B_OFFSET_X2,			// wire 201
  CH_B_OFFSET_X2,			// wire 202
  CH_B_OFFSET_X2,			// wire 203
  CH_B_OFFSET_X2,			// wire 204
  CH_B_OFFSET_X2,			// wire 205
  CH_B_OFFSET_X2,			// wire 206
  CH_B_OFFSET_X2,			// wire 207
  CH_B_OFFSET_X2,			// wire 208
  CH_B_OFFSET_X2,			// wire 209
  CH_B_OFFSET_X2,			// wire 210
  CH_B_OFFSET_X2,			// wire 211
  CH_B_OFFSET_X2,			// wire 212
  CH_B_OFFSET_X2,			// wire 213
  CH_B_OFFSET_X2,			// wire 214
  CH_B_OFFSET_X2,			// wire 215
  CH_B_OFFSET_X2,			// wire 216
  CH_B_OFFSET_X2,			// wire 217
  CH_B_OFFSET_X2,			// wire 218
  CH_B_OFFSET_X2,			// wire 219
  CH_B_OFFSET_X2,			// wire 220
  CH_B_OFFSET_X2,			// wire 221
  CH_B_OFFSET_X2,			// wire 222
  CH_B_OFFSET_X2,			// wire 223
  CH_B_OFFSET_X2,			// wire 224
  CH_B_OFFSET_X2,			// wire 225
  CH_B_OFFSET_X2,			// wire 226
  CH_B_OFFSET_X2,			// wire 227
  CH_B_OFFSET_X2,			// wire 228
  CH_B_OFFSET_X2,			// wire 229
  CH_B_OFFSET_X2,			// wire 230
  CH_B_OFFSET_X2,			// wire 231
  CH_B_OFFSET_X2,			// wire 232
  CH_B_OFFSET_X2,			// wire 233
  CH_B_OFFSET_X2,			// wire 234
  CH_B_OFFSET_X2,			// wire 235
  CH_B_OFFSET_X2,			// wire 236
  CH_B_OFFSET_X2,			// wire 237
  CH_B_OFFSET_X2,			// wire 238
  CH_B_OFFSET_X2,			// wire 239
  CH_B_OFFSET_X2,			// wire 240
  CH_B_OFFSET_X2,			// wire 241
  CH_B_OFFSET_X2,			// wire 242
  CH_B_OFFSET_X2,			// wire 243
  CH_B_OFFSET_X2,			// wire 244
  CH_B_OFFSET_X2,			// wire 245
  CH_B_OFFSET_X2,			// wire 246
  CH_B_OFFSET_X2,			// wire 247
  CH_B_OFFSET_X2,			// wire 248
  CH_B_OFFSET_X2,			// wire 249
  CH_B_OFFSET_X2,			// wire 250
  CH_B_OFFSET_X2,			// wire 251
  CH_B_OFFSET_X2,			// wire 252
  CH_B_OFFSET_X2,			// wire 253
  CH_B_OFFSET_X2,			// wire 254
  CH_B_OFFSET_X2,			// wire 255
  CH_B_OFFSET_X2,			// wire 256
  CH_B_OFFSET_X2,			// wire 257
  CH_B_OFFSET_X2,			// wire 258
  CH_B_OFFSET_X2,			// wire 259
  CH_B_OFFSET_X2,			// wire 260
  CH_B_OFFSET_X2,			// wire 261
  CH_B_OFFSET_X2,			// wire 262
  CH_B_OFFSET_X2,			// wire 263
  CH_B_OFFSET_X2,			// wire 264
  CH_B_OFFSET_X2,			// wire 265
  CH_B_OFFSET_X2,			// wire 266
  CH_B_OFFSET_X2,			// wire 267
  CH_B_OFFSET_X2,			// wire 268
  CH_B_OFFSET_X2,			// wire 269
  CH_B_OFFSET_X2,			// wire 270
  CH_B_OFFSET_X2,			// wire 271
  CH_B_OFFSET_X2,			// wire 272
  CH_B_OFFSET_X2,			// wire 273
  CH_B_OFFSET_X2,			// wire 274
  CH_B_OFFSET_X2,			// wire 275
  CH_B_OFFSET_X2,			// wire 276
  CH_B_OFFSET_X2,			// wire 277
  CH_B_OFFSET_X2,			// wire 278
  CH_B_OFFSET_X2,			// wire 279
  CH_B_OFFSET_X2,			// wire 280
  CH_B_OFFSET_X2,			// wire 281
  CH_B_OFFSET_X2,			// wire 282
  CH_B_OFFSET_X2,			// wire 283
  CH_B_OFFSET_X2,			// wire 284
  CH_B_OFFSET_X2,			// wire 285
  CH_B_OFFSET_X2,			// wire 286
  CH_B_OFFSET_X2,			// wire 287
  CH_B_OFFSET_X2,			// wire 288
  CH_B_OFFSET_X2,			// wire 289
  CH_B_OFFSET_X2,			// wire 290
  CH_B_OFFSET_X2,			// wire 291
  CH_B_OFFSET_X2,			// wire 292
  CH_B_OFFSET_X2,			// wire 293
  CH_B_OFFSET_X2,			// wire 294
  CH_B_OFFSET_X2,			// wire 295
  CH_B_OFFSET_X2,			// wire 296
  CH_B_OFFSET_X2,			// wire 297
  CH_B_OFFSET_X2,			// wire 298
  CH_B_OFFSET_X2,			// wire 299
  CH_B_OFFSET_X2,			// wire 300
  CH_B_OFFSET_X2,			// wire 301
  CH_B_OFFSET_X2,			// wire 302
  CH_B_OFFSET_X2,			// wire 303
  CH_B_OFFSET_X2,			// wire 304
  CH_B_OFFSET_X2,			// wire 305
  CH_B_OFFSET_X2,			// wire 306
  CH_B_OFFSET_X2,			// wire 307
  CH_B_OFFSET_X2,			// wire 308
  CH_B_OFFSET_X2,			// wire 309
  CH_B_OFFSET_X2,			// wire 310
  CH_B_OFFSET_X2,			// wire 311
  CH_B_OFFSET_X2,			// wire 312
  CH_B_OFFSET_X2,			// wire 313
  CH_B_OFFSET_X2,			// wire 314
  CH_B_OFFSET_X2,			// wire 315
  CH_B_OFFSET_X2,			// wire 316
  CH_B_OFFSET_X2,			// wire 317
  CH_B_OFFSET_X2,			// wire 318
  CH_B_OFFSET_X2,			// wire 319
  CH_B_OFFSET_X2,			// wire 320
  CH_B_OFFSET_X2,			// wire 321
  CH_B_OFFSET_X2,			// wire 322
  CH_B_OFFSET_X2,			// wire 323
  CH_B_OFFSET_X2,			// wire 324
  CH_B_OFFSET_X2,			// wire 325
  CH_B_OFFSET_X2,			// wire 326
  CH_B_OFFSET_X2,			// wire 327
  CH_B_OFFSET_X2,			// wire 328
  CH_B_OFFSET_X2,			// wire 329
  CH_B_OFFSET_X2,			// wire 330
  CH_B_OFFSET_X2,			// wire 331
  CH_B_OFFSET_X2,			// wire 332
  CH_B_OFFSET_X2,			// wire 333
  CH_B_OFFSET_X2,			// wire 334
  CH_B_OFFSET_X2,			// wire 335
  CH_B_OFFSET_X2,			// wire 336
  CH_B_OFFSET_X2,			// wire 337
  CH_B_OFFSET_X2,			// wire 338
  CH_B_OFFSET_X2,			// wire 339
  CH_B_OFFSET_X2,			// wire 340
  CH_B_OFFSET_X2,			// wire 341
  CH_B_OFFSET_X2,			// wire 342
  CH_B_OFFSET_X2,			// wire 343
  CH_B_OFFSET_X2,			// wire 344
  CH_B_OFFSET_X2,			// wire 345
  CH_B_OFFSET_X2,			// wire 346
  CH_B_OFFSET_X2,			// wire 347
  CH_B_OFFSET_X2,			// wire 348
  CH_B_OFFSET_X2,			// wire 349
  CH_B_OFFSET_X2,			// wire 350
  CH_B_OFFSET_X2,			// wire 351
  CH_B_OFFSET_X2,			// wire 352
  CH_B_OFFSET_X2,			// wire 353
  CH_B_OFFSET_X2,			// wire 354
  CH_B_OFFSET_X2,			// wire 355
  CH_B_OFFSET_X2,			// wire 356
  CH_B_OFFSET_X2,			// wire 357
  CH_B_OFFSET_X2,			// wire 358
  CH_B_OFFSET_X2,			// wire 359
  CH_B_OFFSET_X2,			// wire 360
  CH_B_OFFSET_X2,			// wire 361
  CH_B_OFFSET_X2,			// wire 362
  CH_B_OFFSET_X2,			// wire 363
  CH_B_OFFSET_X2,			// wire 364
  CH_B_OFFSET_X2,			// wire 365
  CH_B_OFFSET_X2,			// wire 366
  CH_B_OFFSET_X2,			// wire 367
  CH_B_OFFSET_X2,			// wire 368
  CH_B_OFFSET_X2,			// wire 369
  CH_B_OFFSET_X2,			// wire 370
  CH_B_OFFSET_X2,			// wire 371
  CH_B_OFFSET_X2,			// wire 372
  CH_B_OFFSET_X2,			// wire 373
  CH_B_OFFSET_X2,			// wire 374
  CH_B_OFFSET_X2,			// wire 375
  CH_B_OFFSET_X2,			// wire 376
  CH_B_OFFSET_X2,			// wire 377
  CH_B_OFFSET_X2,			// wire 378
  CH_B_OFFSET_X2,			// wire 379
  CH_B_OFFSET_X2,			// wire 380
  CH_B_OFFSET_X2,			// wire 381
  CH_B_OFFSET_X2,			// wire 382
  CH_B_OFFSET_X2,			// wire 383
  CH_B_OFFSET_X2,			// wire 384
  CH_B_OFFSET_X2,			// wire 385
  CH_B_OFFSET_X2,			// wire 386
  CH_B_OFFSET_X2,			// wire 387
  CH_B_OFFSET_X2,			// wire 388
  CH_B_OFFSET_X2,			// wire 389
  CH_B_OFFSET_X2,			// wire 390
  CH_B_OFFSET_X2,			// wire 391
  CH_B_OFFSET_X2,			// wire 392
  CH_B_OFFSET_X2,			// wire 393
  CH_B_OFFSET_X2,			// wire 394
  CH_B_OFFSET_X2,			// wire 395
  CH_B_OFFSET_X2,			// wire 396
  CH_B_OFFSET_X2,			// wire 397
  CH_B_OFFSET_X2,			// wire 398
  CH_B_OFFSET_X2,			// wire 399
  CH_B_OFFSET_X2			// wire 400
};

static short
offset_bs2[CH_B_MAX_S2] = {
  CH_B_OFFSET_S2,			// wire 1
  CH_B_OFFSET_S2,			// wire 2
  CH_B_OFFSET_S2,			// wire 3
  CH_B_OFFSET_S2,			// wire 4
  CH_B_OFFSET_S2,			// wire 5
  CH_B_OFFSET_S2,			// wire 6
  CH_B_OFFSET_S2,			// wire 7
  CH_B_OFFSET_S2,			// wire 8
  CH_B_OFFSET_S2,			// wire 9
  CH_B_OFFSET_S2,			// wire 10
  CH_B_OFFSET_S2,			// wire 11
  CH_B_OFFSET_S2,			// wire 12
  CH_B_OFFSET_S2,			// wire 13
  CH_B_OFFSET_S2,			// wire 14
  CH_B_OFFSET_S2,			// wire 15
  CH_B_OFFSET_S2,			// wire 16
  CH_B_OFFSET_S2,			// wire 17
  CH_B_OFFSET_S2,			// wire 18
  CH_B_OFFSET_S2,			// wire 19
  CH_B_OFFSET_S2,			// wire 20
  CH_B_OFFSET_S2,			// wire 21
  CH_B_OFFSET_S2,			// wire 22
  CH_B_OFFSET_S2,			// wire 23
  CH_B_OFFSET_S2,			// wire 24
  CH_B_OFFSET_S2,			// wire 25
  CH_B_OFFSET_S2,			// wire 26
  CH_B_OFFSET_S2,			// wire 27
  CH_B_OFFSET_S2,			// wire 28
  CH_B_OFFSET_S2,			// wire 29
  CH_B_OFFSET_S2,			// wire 30
  CH_B_OFFSET_S2,			// wire 31
  CH_B_OFFSET_S2,			// wire 32
  CH_B_OFFSET_S2,			// wire 33
  CH_B_OFFSET_S2,			// wire 34
  CH_B_OFFSET_S2,			// wire 35
  CH_B_OFFSET_S2,			// wire 36
  CH_B_OFFSET_S2,			// wire 37
  CH_B_OFFSET_S2,			// wire 38
  CH_B_OFFSET_S2,			// wire 39
  CH_B_OFFSET_S2,			// wire 40
  CH_B_OFFSET_S2,			// wire 41
  CH_B_OFFSET_S2,			// wire 42
  CH_B_OFFSET_S2,			// wire 43
  CH_B_OFFSET_S2,			// wire 44
  CH_B_OFFSET_S2,			// wire 45
  CH_B_OFFSET_S2,			// wire 46
  CH_B_OFFSET_S2,			// wire 47
  CH_B_OFFSET_S2,			// wire 48
  CH_B_OFFSET_S2,			// wire 49
  CH_B_OFFSET_S2,			// wire 50
  CH_B_OFFSET_S2,			// wire 51
  CH_B_OFFSET_S2,			// wire 52
  CH_B_OFFSET_S2,			// wire 53
  CH_B_OFFSET_S2,			// wire 54
  CH_B_OFFSET_S2,			// wire 55
  CH_B_OFFSET_S2,			// wire 56
  CH_B_OFFSET_S2,			// wire 57
  CH_B_OFFSET_S2,			// wire 58
  CH_B_OFFSET_S2,			// wire 59
  CH_B_OFFSET_S2,			// wire 60
  CH_B_OFFSET_S2,			// wire 61
  CH_B_OFFSET_S2,			// wire 62
  CH_B_OFFSET_S2,			// wire 63
  CH_B_OFFSET_S2,			// wire 64
  CH_B_OFFSET_S2,			// wire 65
  CH_B_OFFSET_S2,			// wire 66
  CH_B_OFFSET_S2,			// wire 67
  CH_B_OFFSET_S2,			// wire 68
  CH_B_OFFSET_S2,			// wire 69
  CH_B_OFFSET_S2,			// wire 70
  CH_B_OFFSET_S2,			// wire 71
  CH_B_OFFSET_S2,			// wire 72
  CH_B_OFFSET_S2,			// wire 73
  CH_B_OFFSET_S2,			// wire 74
  CH_B_OFFSET_S2,			// wire 75
  CH_B_OFFSET_S2,			// wire 76
  CH_B_OFFSET_S2,			// wire 77
  CH_B_OFFSET_S2,			// wire 78
  CH_B_OFFSET_S2,			// wire 79
  CH_B_OFFSET_S2,			// wire 80
  CH_B_OFFSET_S2,			// wire 81
  CH_B_OFFSET_S2,			// wire 82
  CH_B_OFFSET_S2,			// wire 83
  CH_B_OFFSET_S2,			// wire 84
  CH_B_OFFSET_S2,			// wire 85
  CH_B_OFFSET_S2,			// wire 86
  CH_B_OFFSET_S2,			// wire 87
  CH_B_OFFSET_S2,			// wire 88
  CH_B_OFFSET_S2,			// wire 89
  CH_B_OFFSET_S2,			// wire 90
  CH_B_OFFSET_S2,			// wire 91
  CH_B_OFFSET_S2,			// wire 92
  CH_B_OFFSET_S2,			// wire 93
  CH_B_OFFSET_S2,			// wire 94
  CH_B_OFFSET_S2,			// wire 95
  CH_B_OFFSET_S2,			// wire 96
  CH_B_OFFSET_S2,			// wire 97
  CH_B_OFFSET_S2,			// wire 98
  CH_B_OFFSET_S2,			// wire 99
  CH_B_OFFSET_S2,			// wire 100
  CH_B_OFFSET_S2,			// wire 101
  CH_B_OFFSET_S2,			// wire 102
  CH_B_OFFSET_S2,			// wire 103
  CH_B_OFFSET_S2,			// wire 104
  CH_B_OFFSET_S2,			// wire 105
  CH_B_OFFSET_S2,			// wire 106
  CH_B_OFFSET_S2,			// wire 107
  CH_B_OFFSET_S2,			// wire 108
  CH_B_OFFSET_S2,			// wire 109
  CH_B_OFFSET_S2,			// wire 110
  CH_B_OFFSET_S2,			// wire 111
  CH_B_OFFSET_S2,			// wire 112
  CH_B_OFFSET_S2,			// wire 113
  CH_B_OFFSET_S2,			// wire 114
  CH_B_OFFSET_S2,			// wire 115
  CH_B_OFFSET_S2,			// wire 116
  CH_B_OFFSET_S2,			// wire 117
  CH_B_OFFSET_S2,			// wire 118
  CH_B_OFFSET_S2,			// wire 119
  CH_B_OFFSET_S2,			// wire 120
  CH_B_OFFSET_S2,			// wire 121
  CH_B_OFFSET_S2,			// wire 122
  CH_B_OFFSET_S2,			// wire 123
  CH_B_OFFSET_S2,			// wire 124
  CH_B_OFFSET_S2,			// wire 125
  CH_B_OFFSET_S2,			// wire 126
  CH_B_OFFSET_S2,			// wire 127
  CH_B_OFFSET_S2,			// wire 128
  CH_B_OFFSET_S2,			// wire 129
  CH_B_OFFSET_S2,			// wire 130
  CH_B_OFFSET_S2,			// wire 131
  CH_B_OFFSET_S2,			// wire 132
  CH_B_OFFSET_S2,			// wire 133
  CH_B_OFFSET_S2,			// wire 134
  CH_B_OFFSET_S2,			// wire 135
  CH_B_OFFSET_S2,			// wire 136
  CH_B_OFFSET_S2,			// wire 137
  CH_B_OFFSET_S2,			// wire 138
  CH_B_OFFSET_S2,			// wire 139
  CH_B_OFFSET_S2,			// wire 140
  CH_B_OFFSET_S2,			// wire 141
  CH_B_OFFSET_S2,			// wire 142
  CH_B_OFFSET_S2,			// wire 143
  CH_B_OFFSET_S2,			// wire 144
  CH_B_OFFSET_S2,			// wire 145
  CH_B_OFFSET_S2,			// wire 146
  CH_B_OFFSET_S2,			// wire 147
  CH_B_OFFSET_S2,			// wire 148
  CH_B_OFFSET_S2,			// wire 149
  CH_B_OFFSET_S2,			// wire 150
  CH_B_OFFSET_S2,			// wire 151
  CH_B_OFFSET_S2,			// wire 152
  CH_B_OFFSET_S2,			// wire 153
  CH_B_OFFSET_S2,			// wire 154
  CH_B_OFFSET_S2,			// wire 155
  CH_B_OFFSET_S2,			// wire 156
  CH_B_OFFSET_S2,			// wire 157
  CH_B_OFFSET_S2,			// wire 158
  CH_B_OFFSET_S2,			// wire 159
  CH_B_OFFSET_S2,			// wire 160
  CH_B_OFFSET_S2,			// wire 161
  CH_B_OFFSET_S2,			// wire 162
  CH_B_OFFSET_S2,			// wire 163
  CH_B_OFFSET_S2,			// wire 164
  CH_B_OFFSET_S2,			// wire 165
  CH_B_OFFSET_S2,			// wire 166
  CH_B_OFFSET_S2,			// wire 167
  CH_B_OFFSET_S2,			// wire 168
  CH_B_OFFSET_S2,			// wire 169
  CH_B_OFFSET_S2,			// wire 170
  CH_B_OFFSET_S2,			// wire 171
  CH_B_OFFSET_S2,			// wire 172
  CH_B_OFFSET_S2,			// wire 173
  CH_B_OFFSET_S2,			// wire 174
  CH_B_OFFSET_S2,			// wire 175
  CH_B_OFFSET_S2,			// wire 176
  CH_B_OFFSET_S2,			// wire 177
  CH_B_OFFSET_S2,			// wire 178
  CH_B_OFFSET_S2,			// wire 179
  CH_B_OFFSET_S2,			// wire 180
  CH_B_OFFSET_S2,			// wire 181
  CH_B_OFFSET_S2,			// wire 182
  CH_B_OFFSET_S2,			// wire 183
  CH_B_OFFSET_S2,			// wire 184
  CH_B_OFFSET_S2,			// wire 185
  CH_B_OFFSET_S2,			// wire 186
  CH_B_OFFSET_S2,			// wire 187
  CH_B_OFFSET_S2,			// wire 188
  CH_B_OFFSET_S2,			// wire 189
  CH_B_OFFSET_S2,			// wire 190
  CH_B_OFFSET_S2,			// wire 191
  CH_B_OFFSET_S2,			// wire 192
  CH_B_OFFSET_S2,			// wire 193
  CH_B_OFFSET_S2,			// wire 194
  CH_B_OFFSET_S2,			// wire 195
  CH_B_OFFSET_S2,			// wire 196
  CH_B_OFFSET_S2,			// wire 197
  CH_B_OFFSET_S2,			// wire 198
  CH_B_OFFSET_S2,			// wire 199
  CH_B_OFFSET_S2,			// wire 200
  CH_B_OFFSET_S2,			// wire 201
  CH_B_OFFSET_S2,			// wire 202
  CH_B_OFFSET_S2,			// wire 203
  CH_B_OFFSET_S2,			// wire 204
  CH_B_OFFSET_S2,			// wire 205
  CH_B_OFFSET_S2,			// wire 206
  CH_B_OFFSET_S2,			// wire 207
  CH_B_OFFSET_S2,			// wire 208
  CH_B_OFFSET_S2,			// wire 209
  CH_B_OFFSET_S2,			// wire 210
  CH_B_OFFSET_S2,			// wire 211
  CH_B_OFFSET_S2,			// wire 212
  CH_B_OFFSET_S2,			// wire 213
  CH_B_OFFSET_S2,			// wire 214
  CH_B_OFFSET_S2,			// wire 215
  CH_B_OFFSET_S2,			// wire 216
  CH_B_OFFSET_S2,			// wire 217
  CH_B_OFFSET_S2,			// wire 218
  CH_B_OFFSET_S2,			// wire 219
  CH_B_OFFSET_S2,			// wire 220
  CH_B_OFFSET_S2,			// wire 221
  CH_B_OFFSET_S2,			// wire 222
  CH_B_OFFSET_S2,			// wire 223
  CH_B_OFFSET_S2,			// wire 224
  CH_B_OFFSET_S2,			// wire 225
  CH_B_OFFSET_S2,			// wire 226
  CH_B_OFFSET_S2,			// wire 227
  CH_B_OFFSET_S2,			// wire 228
  CH_B_OFFSET_S2,			// wire 229
  CH_B_OFFSET_S2,			// wire 230
  CH_B_OFFSET_S2,			// wire 231
  CH_B_OFFSET_S2,			// wire 232
  CH_B_OFFSET_S2,			// wire 233
  CH_B_OFFSET_S2,			// wire 234
  CH_B_OFFSET_S2,			// wire 235
  CH_B_OFFSET_S2,			// wire 236
  CH_B_OFFSET_S2,			// wire 237
  CH_B_OFFSET_S2,			// wire 238
  CH_B_OFFSET_S2,			// wire 239
  CH_B_OFFSET_S2,			// wire 240
  CH_B_OFFSET_S2,			// wire 241
  CH_B_OFFSET_S2,			// wire 242
  CH_B_OFFSET_S2,			// wire 243
  CH_B_OFFSET_S2,			// wire 244
  CH_B_OFFSET_S2,			// wire 245
  CH_B_OFFSET_S2,			// wire 246
  CH_B_OFFSET_S2,			// wire 247
  CH_B_OFFSET_S2,			// wire 248
  CH_B_OFFSET_S2,			// wire 249
  CH_B_OFFSET_S2,			// wire 250
  CH_B_OFFSET_S2,			// wire 251
  CH_B_OFFSET_S2,			// wire 252
  CH_B_OFFSET_S2,			// wire 253
  CH_B_OFFSET_S2,			// wire 254
  CH_B_OFFSET_S2,			// wire 255
  CH_B_OFFSET_S2,			// wire 256
  CH_B_OFFSET_S2,			// wire 257
  CH_B_OFFSET_S2,			// wire 258
  CH_B_OFFSET_S2,			// wire 259
  CH_B_OFFSET_S2,			// wire 260
  CH_B_OFFSET_S2,			// wire 261
  CH_B_OFFSET_S2,			// wire 262
  CH_B_OFFSET_S2,			// wire 263
  CH_B_OFFSET_S2,			// wire 264
  CH_B_OFFSET_S2,			// wire 265
  CH_B_OFFSET_S2,			// wire 266
  CH_B_OFFSET_S2,			// wire 267
  CH_B_OFFSET_S2,			// wire 268
  CH_B_OFFSET_S2,			// wire 269
  CH_B_OFFSET_S2,			// wire 270
  CH_B_OFFSET_S2,			// wire 271
  CH_B_OFFSET_S2,			// wire 272
  CH_B_OFFSET_S2,			// wire 273
  CH_B_OFFSET_S2,			// wire 274
  CH_B_OFFSET_S2,			// wire 275
  CH_B_OFFSET_S2,			// wire 276
  CH_B_OFFSET_S2,			// wire 277
  CH_B_OFFSET_S2,			// wire 278
  CH_B_OFFSET_S2,			// wire 279
  CH_B_OFFSET_S2,			// wire 280
  CH_B_OFFSET_S2,			// wire 281
  CH_B_OFFSET_S2,			// wire 282
  CH_B_OFFSET_S2,			// wire 283
  CH_B_OFFSET_S2,			// wire 284
  CH_B_OFFSET_S2,			// wire 285
  CH_B_OFFSET_S2,			// wire 286
  CH_B_OFFSET_S2,			// wire 287
  CH_B_OFFSET_S2,			// wire 288
  CH_B_OFFSET_S2,			// wire 289
  CH_B_OFFSET_S2,			// wire 290
  CH_B_OFFSET_S2,			// wire 291
  CH_B_OFFSET_S2,			// wire 292
  CH_B_OFFSET_S2,			// wire 293
  CH_B_OFFSET_S2,			// wire 294
  CH_B_OFFSET_S2,			// wire 295
  CH_B_OFFSET_S2,			// wire 296
  CH_B_OFFSET_S2,			// wire 297
  CH_B_OFFSET_S2,			// wire 298
  CH_B_OFFSET_S2,			// wire 299
  CH_B_OFFSET_S2,			// wire 300
  CH_B_OFFSET_S2,			// wire 301
  CH_B_OFFSET_S2,			// wire 302
  CH_B_OFFSET_S2,			// wire 303
  CH_B_OFFSET_S2,			// wire 304
  CH_B_OFFSET_S2,			// wire 305
  CH_B_OFFSET_S2,			// wire 306
  CH_B_OFFSET_S2,			// wire 307
  CH_B_OFFSET_S2,			// wire 308
  CH_B_OFFSET_S2,			// wire 309
  CH_B_OFFSET_S2,			// wire 310
  CH_B_OFFSET_S2,			// wire 311
  CH_B_OFFSET_S2,			// wire 312
  CH_B_OFFSET_S2,			// wire 313
  CH_B_OFFSET_S2,			// wire 314
  CH_B_OFFSET_S2,			// wire 315
  CH_B_OFFSET_S2,			// wire 316
  CH_B_OFFSET_S2,			// wire 317
  CH_B_OFFSET_S2,			// wire 318
  CH_B_OFFSET_S2,			// wire 319
  CH_B_OFFSET_S2,			// wire 320
  CH_B_OFFSET_S2,			// wire 321
  CH_B_OFFSET_S2,			// wire 322
  CH_B_OFFSET_S2,			// wire 323
  CH_B_OFFSET_S2,			// wire 324
  CH_B_OFFSET_S2,			// wire 325
  CH_B_OFFSET_S2,			// wire 326
  CH_B_OFFSET_S2,			// wire 327
  CH_B_OFFSET_S2,			// wire 328
  CH_B_OFFSET_S2,			// wire 329
  CH_B_OFFSET_S2,			// wire 330
  CH_B_OFFSET_S2,			// wire 331
  CH_B_OFFSET_S2,			// wire 332
  CH_B_OFFSET_S2,			// wire 333
  CH_B_OFFSET_S2,			// wire 334
  CH_B_OFFSET_S2,			// wire 335
  CH_B_OFFSET_S2,			// wire 336
  CH_B_OFFSET_S2,			// wire 337
  CH_B_OFFSET_S2,			// wire 338
  CH_B_OFFSET_S2,			// wire 339
  CH_B_OFFSET_S2,			// wire 340
  CH_B_OFFSET_S2,			// wire 341
  CH_B_OFFSET_S2,			// wire 342
  CH_B_OFFSET_S2,			// wire 343
  CH_B_OFFSET_S2,			// wire 344
  CH_B_OFFSET_S2,			// wire 345
  CH_B_OFFSET_S2,			// wire 346
  CH_B_OFFSET_S2,			// wire 347
  CH_B_OFFSET_S2,			// wire 348
  CH_B_OFFSET_S2,			// wire 349
  CH_B_OFFSET_S2,			// wire 350
  CH_B_OFFSET_S2,			// wire 351
  CH_B_OFFSET_S2,			// wire 352
  CH_B_OFFSET_S2,			// wire 353
  CH_B_OFFSET_S2,			// wire 354
  CH_B_OFFSET_S2,			// wire 355
  CH_B_OFFSET_S2,			// wire 356
  CH_B_OFFSET_S2,			// wire 357
  CH_B_OFFSET_S2,			// wire 358
  CH_B_OFFSET_S2,			// wire 359
  CH_B_OFFSET_S2,			// wire 360
  CH_B_OFFSET_S2,			// wire 361
  CH_B_OFFSET_S2,			// wire 362
  CH_B_OFFSET_S2,			// wire 363
  CH_B_OFFSET_S2,			// wire 364
  CH_B_OFFSET_S2,			// wire 365
  CH_B_OFFSET_S2,			// wire 366
  CH_B_OFFSET_S2,			// wire 367
  CH_B_OFFSET_S2			// wire 368
};

static float
position_bx1[CH_B_MAX_X1] = {
  -883.00,		// x1 wire 1
  -878.00,		// x1 wire 2
  -873.00,		// x1 wire 3
  -868.00,		// x1 wire 4
  -863.00,		// x1 wire 5
  -858.00,		// x1 wire 6
  -853.00,		// x1 wire 7
  -848.00,		// x1 wire 8
  -843.00,		// x1 wire 9
  -838.00,		// x1 wire 10
  -833.00,		// x1 wire 11
  -828.00,		// x1 wire 12
  -823.00,		// x1 wire 13
  -818.00,		// x1 wire 14
  -813.00,		// x1 wire 15
  -808.00,		// x1 wire 16
  -803.00,		// x1 wire 17
  -798.00,		// x1 wire 18
  -793.00,		// x1 wire 19
  -788.00,		// x1 wire 20
  -783.00,		// x1 wire 21
  -778.00,		// x1 wire 22
  -773.00,		// x1 wire 23
  -768.00,		// x1 wire 24
  -763.00,		// x1 wire 25
  -758.00,		// x1 wire 26
  -753.00,		// x1 wire 27
  -748.00,		// x1 wire 28
  -743.00,		// x1 wire 29
  -738.00,		// x1 wire 30
  -733.00,		// x1 wire 31
  -728.00,		// x1 wire 32
  -723.00,		// x1 wire 33
  -718.00,		// x1 wire 34
  -713.00,		// x1 wire 35
  -708.00,		// x1 wire 36
  -703.00,		// x1 wire 37
  -698.00,		// x1 wire 38
  -693.00,		// x1 wire 39
  -688.00,		// x1 wire 40
  -683.00,		// x1 wire 41
  -678.00,		// x1 wire 42
  -673.00,		// x1 wire 43
  -668.00,		// x1 wire 44
  -663.00,		// x1 wire 45
  -658.00,		// x1 wire 46
  -653.00,		// x1 wire 47
  -648.00,		// x1 wire 48
  -643.00,		// x1 wire 49
  -638.00,		// x1 wire 50
  -633.00,		// x1 wire 51
  -628.00,		// x1 wire 52
  -623.00,		// x1 wire 53
  -618.00,		// x1 wire 54
  -613.00,		// x1 wire 55
  -608.00,		// x1 wire 56
  -603.00,		// x1 wire 57
  -598.00,		// x1 wire 58
  -593.00,		// x1 wire 59
  -588.00,		// x1 wire 60
  -583.00,		// x1 wire 61
  -578.00,		// x1 wire 62
  -573.00,		// x1 wire 63
  -568.00,		// x1 wire 64
  -563.00,		// x1 wire 65
  -558.00,		// x1 wire 66
  -553.00,		// x1 wire 67
  -548.00,		// x1 wire 68
  -543.00,		// x1 wire 69
  -538.00,		// x1 wire 70
  -533.00,		// x1 wire 71
  -528.00,		// x1 wire 72
  -523.00,		// x1 wire 73
  -518.00,		// x1 wire 74
  -513.00,		// x1 wire 75
  -508.00,		// x1 wire 76
  -503.00,		// x1 wire 77
  -498.00,		// x1 wire 78
  -493.00,		// x1 wire 79
  -488.00,		// x1 wire 80
  -483.00,		// x1 wire 81
  -478.00,		// x1 wire 82
  -473.00,		// x1 wire 83
  -468.00,		// x1 wire 84
  -463.00,		// x1 wire 85
  -458.00,		// x1 wire 86
  -453.00,		// x1 wire 87
  -448.00,		// x1 wire 88
  -443.00,		// x1 wire 89
  -438.00,		// x1 wire 90
  -433.00,		// x1 wire 91
  -428.00,		// x1 wire 92
  -423.00,		// x1 wire 93
  -418.00,		// x1 wire 94
  -413.00,		// x1 wire 95
  -408.00,		// x1 wire 96
  -403.00,		// x1 wire 97
  -398.00,		// x1 wire 98
  -393.00,		// x1 wire 99
  -388.00,		// x1 wire 100
  -383.00,		// x1 wire 101
  -378.00,		// x1 wire 102
  -373.00,		// x1 wire 103
  -368.00,		// x1 wire 104
  -363.00,		// x1 wire 105
  -358.00,		// x1 wire 106
  -353.00,		// x1 wire 107
  -348.00,		// x1 wire 108
  -343.00,		// x1 wire 109
  -338.00,		// x1 wire 110
  -333.00,		// x1 wire 111
  -328.00,		// x1 wire 112
  -323.00,		// x1 wire 113
  -318.00,		// x1 wire 114
  -313.00,		// x1 wire 115
  -308.00,		// x1 wire 116
  -303.00,		// x1 wire 117
  -298.00,		// x1 wire 118
  -293.00,		// x1 wire 119
  -288.00,		// x1 wire 120
  -283.00,		// x1 wire 121
  -278.00,		// x1 wire 122
  -273.00,		// x1 wire 123
  -268.00,		// x1 wire 124
  -263.00,		// x1 wire 125
  -258.00,		// x1 wire 126
  -253.00,		// x1 wire 127
  -248.00,		// x1 wire 128
  -243.00,		// x1 wire 129
  -238.00,		// x1 wire 130
  -233.00,		// x1 wire 131
  -228.00,		// x1 wire 132
  -223.00,		// x1 wire 133
  -218.00,		// x1 wire 134
  -213.00,		// x1 wire 135
  -208.00,		// x1 wire 136
  -203.00,		// x1 wire 137
  -198.00,		// x1 wire 138
  -193.00,		// x1 wire 139
  -188.00,		// x1 wire 140
  -183.00,		// x1 wire 141
  -178.00,		// x1 wire 142
  -173.00,		// x1 wire 143
  -168.00,		// x1 wire 144
  -163.00,		// x1 wire 145
  -158.00,		// x1 wire 146
  -153.00,		// x1 wire 147
  -148.00,		// x1 wire 148
  -143.00,		// x1 wire 149
  -138.00,		// x1 wire 150
  -133.00,		// x1 wire 151
  -128.00,		// x1 wire 152
  -123.00,		// x1 wire 153
  -118.00,		// x1 wire 154
  -113.00,		// x1 wire 155
  -108.00,		// x1 wire 156
  -103.00,		// x1 wire 157
   -98.00,		// x1 wire 158
   -93.00,		// x1 wire 159
   -88.00,		// x1 wire 160
   -83.00,		// x1 wire 161
   -78.00,		// x1 wire 162
   -73.00,		// x1 wire 163
   -68.00,		// x1 wire 164
   -63.00,		// x1 wire 165
   -58.00,		// x1 wire 166
   -53.00,		// x1 wire 167
   -48.00,		// x1 wire 168
   -43.00,		// x1 wire 169
   -38.00,		// x1 wire 170
   -33.00,		// x1 wire 171
   -28.00,		// x1 wire 172
   -23.00,		// x1 wire 173
   -18.00,		// x1 wire 174
   -13.00,		// x1 wire 175
    -8.00,		// x1 wire 176
    -3.00,		// x1 wire 177
     2.00,		// x1 wire 178
     7.00,		// x1 wire 179
    12.00,		// x1 wire 180
    17.00,		// x1 wire 181
    22.00,		// x1 wire 182
    27.00,		// x1 wire 183
    32.00,		// x1 wire 184
    37.00,		// x1 wire 185
    42.00,		// x1 wire 186
    47.00,		// x1 wire 187
    52.00,		// x1 wire 188
    57.00,		// x1 wire 189
    62.00,		// x1 wire 190
    67.00,		// x1 wire 191
    72.00,		// x1 wire 192
    77.00,		// x1 wire 193
    82.00,		// x1 wire 194
    87.00,		// x1 wire 195
    92.00,		// x1 wire 196
    97.00,		// x1 wire 197
   102.00,		// x1 wire 198
   107.00,		// x1 wire 199
   112.00,		// x1 wire 200
   117.00,		// x1 wire 201
   122.00,		// x1 wire 202
   127.00,		// x1 wire 203
   132.00,		// x1 wire 204
   137.00,		// x1 wire 205
   142.00,		// x1 wire 206
   147.00,		// x1 wire 207
   152.00,		// x1 wire 208
   157.00,		// x1 wire 209
   162.00,		// x1 wire 210
   167.00,		// x1 wire 211
   172.00,		// x1 wire 212
   177.00,		// x1 wire 213
   182.00,		// x1 wire 214
   187.00,		// x1 wire 215
   192.00,		// x1 wire 216
   197.00,		// x1 wire 217
   202.00,		// x1 wire 218
   207.00,		// x1 wire 219
   212.00,		// x1 wire 220
   217.00,		// x1 wire 221
   222.00,		// x1 wire 222
   227.00,		// x1 wire 223
   232.00,		// x1 wire 224
   237.00,		// x1 wire 225
   242.00,		// x1 wire 226
   247.00,		// x1 wire 227
   252.00,		// x1 wire 228
   257.00,		// x1 wire 229
   262.00,		// x1 wire 230
   267.00,		// x1 wire 231
   272.00,		// x1 wire 232
   277.00,		// x1 wire 233
   282.00,		// x1 wire 234
   287.00,		// x1 wire 235
   292.00,		// x1 wire 236
   297.00,		// x1 wire 237
   302.00,		// x1 wire 238
   307.00,		// x1 wire 239
   312.00,		// x1 wire 240
   317.00,		// x1 wire 241
   322.00,		// x1 wire 242
   327.00,		// x1 wire 243
   332.00,		// x1 wire 244
   337.00,		// x1 wire 245
   342.00,		// x1 wire 246
   347.00,		// x1 wire 247
   352.00,		// x1 wire 248
   357.00,		// x1 wire 249
   362.00,		// x1 wire 250
   367.00,		// x1 wire 251
   372.00,		// x1 wire 252
   377.00,		// x1 wire 253
   382.00,		// x1 wire 254
   387.00,		// x1 wire 255
   392.00,		// x1 wire 256
   397.00,		// x1 wire 257
   402.00,		// x1 wire 258
   407.00,		// x1 wire 259
   412.00,		// x1 wire 260
   417.00,		// x1 wire 261
   422.00,		// x1 wire 262
   427.00,		// x1 wire 263
   432.00,		// x1 wire 264
   437.00,		// x1 wire 265
   442.00,		// x1 wire 266
   447.00,		// x1 wire 267
   452.00,		// x1 wire 268
   457.00,		// x1 wire 269
   462.00,		// x1 wire 270
   467.00,		// x1 wire 271
   472.00,		// x1 wire 272
   477.00,		// x1 wire 273
   482.00,		// x1 wire 274
   487.00,		// x1 wire 275
   492.00,		// x1 wire 276
   497.00,		// x1 wire 277
   502.00,		// x1 wire 278
   507.00,		// x1 wire 279
   512.00,		// x1 wire 280
   517.00,		// x1 wire 281
   522.00,		// x1 wire 282
   527.00,		// x1 wire 283
   532.00,		// x1 wire 284
   537.00,		// x1 wire 285
   542.00,		// x1 wire 286
   547.00,		// x1 wire 287
   552.00,		// x1 wire 288
   557.00,		// x1 wire 289
   562.00,		// x1 wire 290
   567.00,		// x1 wire 291
   572.00,		// x1 wire 292
   577.00,		// x1 wire 293
   582.00,		// x1 wire 294
   587.00,		// x1 wire 295
   592.00,		// x1 wire 296
   597.00,		// x1 wire 297
   602.00,		// x1 wire 298
   607.00,		// x1 wire 299
   612.00,		// x1 wire 300
   617.00,		// x1 wire 301
   622.00,		// x1 wire 302
   627.00,		// x1 wire 303
   632.00,		// x1 wire 304
   637.00,		// x1 wire 305
   642.00,		// x1 wire 306
   647.00,		// x1 wire 307
   652.00,		// x1 wire 308
   657.00,		// x1 wire 309
   662.00,		// x1 wire 310
   667.00,		// x1 wire 311
   672.00,		// x1 wire 312
   677.00,		// x1 wire 313
   682.00,		// x1 wire 314
   687.00,		// x1 wire 315
   692.00,		// x1 wire 316
   697.00,		// x1 wire 317
   702.00,		// x1 wire 318
   707.00,		// x1 wire 319
   712.00,		// x1 wire 320
   717.00,		// x1 wire 321
   722.00,		// x1 wire 322
   727.00,		// x1 wire 323
   732.00,		// x1 wire 324
   737.00,		// x1 wire 325
   742.00,		// x1 wire 326
   747.00,		// x1 wire 327
   752.00,		// x1 wire 328
   757.00,		// x1 wire 329
   762.00,		// x1 wire 330
   767.00,		// x1 wire 331
   772.00,		// x1 wire 332
   777.00,		// x1 wire 333
   782.00,		// x1 wire 334
   787.00,		// x1 wire 335
   792.00,		// x1 wire 336
   797.00,		// x1 wire 337
   802.00,		// x1 wire 338
   807.00,		// x1 wire 339
   812.00,		// x1 wire 340
   817.00,		// x1 wire 341
   822.00,		// x1 wire 342
   827.00,		// x1 wire 343
   832.00,		// x1 wire 344
   837.00,		// x1 wire 345
   842.00,		// x1 wire 346
   847.00,		// x1 wire 347
   852.00,		// x1 wire 348
   857.00,		// x1 wire 349
   862.00,		// x1 wire 350
   867.00,		// x1 wire 351
   872.00,		// x1 wire 352
   877.00,		// x1 wire 353
   882.00,		// x1 wire 354
   887.00,		// x1 wire 355
   892.00,		// x1 wire 356
   897.00,		// x1 wire 357
   902.00,		// x1 wire 358
   907.00,		// x1 wire 359
   912.00,		// x1 wire 360
   917.00,		// x1 wire 361
   922.00,		// x1 wire 362
   927.00,		// x1 wire 363
   932.00,		// x1 wire 364
   937.00,		// x1 wire 365
   942.00,		// x1 wire 366
   947.00,		// x1 wire 367
   952.00		// x1 wire 368
};

static float
position_bs1[CH_B_MAX_S1] = {
  -868.54,		// s1 wire 1
  -863.54,		// s1 wire 2
  -858.54,		// s1 wire 3
  -853.54,		// s1 wire 4
  -848.54,		// s1 wire 5
  -843.54,		// s1 wire 6
  -838.54,		// s1 wire 7
  -833.54,		// s1 wire 8
  -828.54,		// s1 wire 9
  -823.54,		// s1 wire 10
  -818.54,		// s1 wire 11
  -813.54,		// s1 wire 12
  -808.54,		// s1 wire 13
  -803.54,		// s1 wire 14
  -798.54,		// s1 wire 15
  -793.54,		// s1 wire 16
  -788.54,		// s1 wire 17
  -783.54,		// s1 wire 18
  -778.54,		// s1 wire 19
  -773.54,		// s1 wire 20
  -768.54,		// s1 wire 21
  -763.54,		// s1 wire 22
  -758.54,		// s1 wire 23
  -753.54,		// s1 wire 24
  -748.54,		// s1 wire 25
  -743.54,		// s1 wire 26
  -738.54,		// s1 wire 27
  -733.54,		// s1 wire 28
  -728.54,		// s1 wire 29
  -723.54,		// s1 wire 30
  -718.54,		// s1 wire 31
  -713.54,		// s1 wire 32
  -708.54,		// s1 wire 33
  -703.54,		// s1 wire 34
  -698.54,		// s1 wire 35
  -693.54,		// s1 wire 36
  -688.54,		// s1 wire 37
  -683.54,		// s1 wire 38
  -678.54,		// s1 wire 39
  -673.54,		// s1 wire 40
  -668.54,		// s1 wire 41
  -663.54,		// s1 wire 42
  -658.54,		// s1 wire 43
  -653.54,		// s1 wire 44
  -648.54,		// s1 wire 45
  -643.54,		// s1 wire 46
  -638.54,		// s1 wire 47
  -633.54,		// s1 wire 48
  -628.54,		// s1 wire 49
  -623.54,		// s1 wire 50
  -618.54,		// s1 wire 51
  -613.54,		// s1 wire 52
  -608.54,		// s1 wire 53
  -603.54,		// s1 wire 54
  -598.54,		// s1 wire 55
  -593.54,		// s1 wire 56
  -588.54,		// s1 wire 57
  -583.54,		// s1 wire 58
  -578.54,		// s1 wire 59
  -573.54,		// s1 wire 60
  -568.54,		// s1 wire 61
  -563.54,		// s1 wire 62
  -558.54,		// s1 wire 63
  -553.54,		// s1 wire 64
  -548.54,		// s1 wire 65
  -543.54,		// s1 wire 66
  -538.54,		// s1 wire 67
  -533.54,		// s1 wire 68
  -528.54,		// s1 wire 69
  -523.54,		// s1 wire 70
  -518.54,		// s1 wire 71
  -513.54,		// s1 wire 72
  -508.54,		// s1 wire 73
  -503.54,		// s1 wire 74
  -498.54,		// s1 wire 75
  -493.54,		// s1 wire 76
  -488.54,		// s1 wire 77
  -483.54,		// s1 wire 78
  -478.54,		// s1 wire 79
  -473.54,		// s1 wire 80
  -468.54,		// s1 wire 81
  -463.54,		// s1 wire 82
  -458.54,		// s1 wire 83
  -453.54,		// s1 wire 84
  -448.54,		// s1 wire 85
  -443.54,		// s1 wire 86
  -438.54,		// s1 wire 87
  -433.54,		// s1 wire 88
  -428.54,		// s1 wire 89
  -423.54,		// s1 wire 90
  -418.54,		// s1 wire 91
  -413.54,		// s1 wire 92
  -408.54,		// s1 wire 93
  -403.54,		// s1 wire 94
  -398.54,		// s1 wire 95
  -393.54,		// s1 wire 96
  -388.54,		// s1 wire 97
  -383.54,		// s1 wire 98
  -378.54,		// s1 wire 99
  -373.54,		// s1 wire 100
  -368.54,		// s1 wire 101
  -363.54,		// s1 wire 102
  -358.54,		// s1 wire 103
  -353.54,		// s1 wire 104
  -348.54,		// s1 wire 105
  -343.54,		// s1 wire 106
  -338.54,		// s1 wire 107
  -333.54,		// s1 wire 108
  -328.54,		// s1 wire 109
  -323.54,		// s1 wire 110
  -318.54,		// s1 wire 111
  -313.54,		// s1 wire 112
  -308.54,		// s1 wire 113
  -303.54,		// s1 wire 114
  -298.54,		// s1 wire 115
  -293.54,		// s1 wire 116
  -288.54,		// s1 wire 117
  -283.54,		// s1 wire 118
  -278.54,		// s1 wire 119
  -273.54,		// s1 wire 120
  -268.54,		// s1 wire 121
  -263.54,		// s1 wire 122
  -258.54,		// s1 wire 123
  -253.54,		// s1 wire 124
  -248.54,		// s1 wire 125
  -243.54,		// s1 wire 126
  -238.54,		// s1 wire 127
  -233.54,		// s1 wire 128
  -228.54,		// s1 wire 129
  -223.54,		// s1 wire 130
  -218.54,		// s1 wire 131
  -213.54,		// s1 wire 132
  -208.54,		// s1 wire 133
  -203.54,		// s1 wire 134
  -198.54,		// s1 wire 135
  -193.54,		// s1 wire 136
  -188.54,		// s1 wire 137
  -183.54,		// s1 wire 138
  -178.54,		// s1 wire 139
  -173.54,		// s1 wire 140
  -168.54,		// s1 wire 141
  -163.54,		// s1 wire 142
  -158.54,		// s1 wire 143
  -153.54,		// s1 wire 144
  -148.54,		// s1 wire 145
  -143.54,		// s1 wire 146
  -138.54,		// s1 wire 147
  -133.54,		// s1 wire 148
  -128.54,		// s1 wire 149
  -123.54,		// s1 wire 150
  -118.54,		// s1 wire 151
  -113.54,		// s1 wire 152
  -108.54,		// s1 wire 153
  -103.54,		// s1 wire 154
   -98.54,		// s1 wire 155
   -93.54,		// s1 wire 156
   -88.54,		// s1 wire 157
   -83.54,		// s1 wire 158
   -78.54,		// s1 wire 159
   -73.54,		// s1 wire 160
   -68.54,		// s1 wire 161
   -63.54,		// s1 wire 162
   -58.54,		// s1 wire 163
   -53.54,		// s1 wire 164
   -48.54,		// s1 wire 165
   -43.54,		// s1 wire 166
   -38.54,		// s1 wire 167
   -33.54,		// s1 wire 168
   -28.54,		// s1 wire 169
   -23.54,		// s1 wire 170
   -18.54,		// s1 wire 171
   -13.54,		// s1 wire 172
    -8.54,		// s1 wire 173
    -3.54,		// s1 wire 174
     1.46,		// s1 wire 175
     6.46,		// s1 wire 176
    11.46,		// s1 wire 177
    16.46,		// s1 wire 178
    21.46,		// s1 wire 179
    26.46,		// s1 wire 180
    31.46,		// s1 wire 181
    36.46,		// s1 wire 182
    41.46,		// s1 wire 183
    46.46,		// s1 wire 184
    51.46,		// s1 wire 185
    56.46,		// s1 wire 186
    61.46,		// s1 wire 187
    66.46,		// s1 wire 188
    71.46,		// s1 wire 189
    76.46,		// s1 wire 190
    81.46,		// s1 wire 191
    86.46,		// s1 wire 192
    91.46,		// s1 wire 193
    96.46,		// s1 wire 194
   101.46,		// s1 wire 195
   106.46,		// s1 wire 196
   111.46,		// s1 wire 197
   116.46,		// s1 wire 198
   121.46,		// s1 wire 199
   126.46,		// s1 wire 200
   131.46,		// s1 wire 201
   136.46,		// s1 wire 202
   141.46,		// s1 wire 203
   146.46,		// s1 wire 204
   151.46,		// s1 wire 205
   156.46,		// s1 wire 206
   161.46,		// s1 wire 207
   166.46,		// s1 wire 208
   171.46,		// s1 wire 209
   176.46,		// s1 wire 210
   181.46,		// s1 wire 211
   186.46,		// s1 wire 212
   191.46,		// s1 wire 213
   196.46,		// s1 wire 214
   201.46,		// s1 wire 215
   206.46,		// s1 wire 216
   211.46,		// s1 wire 217
   216.46,		// s1 wire 218
   221.46,		// s1 wire 219
   226.46,		// s1 wire 220
   231.46,		// s1 wire 221
   236.46,		// s1 wire 222
   241.46,		// s1 wire 223
   246.46,		// s1 wire 224
   251.46,		// s1 wire 225
   256.46,		// s1 wire 226
   261.46,		// s1 wire 227
   266.46,		// s1 wire 228
   271.46,		// s1 wire 229
   276.46,		// s1 wire 230
   281.46,		// s1 wire 231
   286.46,		// s1 wire 232
   291.46,		// s1 wire 233
   296.46,		// s1 wire 234
   301.46,		// s1 wire 235
   306.46,		// s1 wire 236
   311.46,		// s1 wire 237
   316.46,		// s1 wire 238
   321.46,		// s1 wire 239
   326.46,		// s1 wire 240
   331.46,		// s1 wire 241
   336.46,		// s1 wire 242
   341.46,		// s1 wire 243
   346.46,		// s1 wire 244
   351.46,		// s1 wire 245
   356.46,		// s1 wire 246
   361.46,		// s1 wire 247
   366.46,		// s1 wire 248
   371.46,		// s1 wire 249
   376.46,		// s1 wire 250
   381.46,		// s1 wire 251
   386.46,		// s1 wire 252
   391.46,		// s1 wire 253
   396.46,		// s1 wire 254
   401.46,		// s1 wire 255
   406.46,		// s1 wire 256
   411.46,		// s1 wire 257
   416.46,		// s1 wire 258
   421.46,		// s1 wire 259
   426.46,		// s1 wire 260
   431.46,		// s1 wire 261
   436.46,		// s1 wire 262
   441.46,		// s1 wire 263
   446.46,		// s1 wire 264
   451.46,		// s1 wire 265
   456.46,		// s1 wire 266
   461.46,		// s1 wire 267
   466.46,		// s1 wire 268
   471.46,		// s1 wire 269
   476.46,		// s1 wire 270
   481.46,		// s1 wire 271
   486.46,		// s1 wire 272
   491.46,		// s1 wire 273
   496.46,		// s1 wire 274
   501.46,		// s1 wire 275
   506.46,		// s1 wire 276
   511.46,		// s1 wire 277
   516.46,		// s1 wire 278
   521.46,		// s1 wire 279
   526.46,		// s1 wire 280
   531.46,		// s1 wire 281
   536.46,		// s1 wire 282
   541.46,		// s1 wire 283
   546.46,		// s1 wire 284
   551.46,		// s1 wire 285
   556.46,		// s1 wire 286
   561.46,		// s1 wire 287
   566.46,		// s1 wire 288
   571.46,		// s1 wire 289
   576.46,		// s1 wire 290
   581.46,		// s1 wire 291
   586.46,		// s1 wire 292
   591.46,		// s1 wire 293
   596.46,		// s1 wire 294
   601.46,		// s1 wire 295
   606.46,		// s1 wire 296
   611.46,		// s1 wire 297
   616.46,		// s1 wire 298
   621.46,		// s1 wire 299
   626.46,		// s1 wire 300
   631.46,		// s1 wire 301
   636.46,		// s1 wire 302
   641.46,		// s1 wire 303
   646.46,		// s1 wire 304
   651.46,		// s1 wire 305
   656.46,		// s1 wire 306
   661.46,		// s1 wire 307
   666.46,		// s1 wire 308
   671.46,		// s1 wire 309
   676.46,		// s1 wire 310
   681.46,		// s1 wire 311
   686.46,		// s1 wire 312
   691.46,		// s1 wire 313
   696.46,		// s1 wire 314
   701.46,		// s1 wire 315
   706.46,		// s1 wire 316
   711.46,		// s1 wire 317
   716.46,		// s1 wire 318
   721.46,		// s1 wire 319
   726.46,		// s1 wire 320
   731.46,		// s1 wire 321
   736.46,		// s1 wire 322
   741.46,		// s1 wire 323
   746.46,		// s1 wire 324
   751.46,		// s1 wire 325
   756.46,		// s1 wire 326
   761.46,		// s1 wire 327
   766.46,		// s1 wire 328
   771.46,		// s1 wire 329
   776.46,		// s1 wire 330
   781.46,		// s1 wire 331
   786.46,		// s1 wire 332
   791.46,		// s1 wire 333
   796.46,		// s1 wire 334
   801.46,		// s1 wire 335
   806.46		// s1 wire 336
};

static float
position_bx2[CH_B_MAX_X2] = {
  -635.00,		// x2 wire 1
  -630.00,		// x2 wire 2
  -625.00,		// x2 wire 3
  -620.00,		// x2 wire 4
  -615.00,		// x2 wire 5
  -610.00,		// x2 wire 6
  -605.00,		// x2 wire 7
  -600.00,		// x2 wire 8
  -595.00,		// x2 wire 9
  -590.00,		// x2 wire 10
  -585.00,		// x2 wire 11
  -580.00,		// x2 wire 12
  -575.00,		// x2 wire 13
  -570.00,		// x2 wire 14
  -565.00,		// x2 wire 15
  -560.00,		// x2 wire 16
  -555.00,		// x2 wire 17
  -550.00,		// x2 wire 18
  -545.00,		// x2 wire 19
  -540.00,		// x2 wire 20
  -535.00,		// x2 wire 21
  -530.00,		// x2 wire 22
  -525.00,		// x2 wire 23
  -520.00,		// x2 wire 24
  -515.00,		// x2 wire 25
  -510.00,		// x2 wire 26
  -505.00,		// x2 wire 27
  -500.00,		// x2 wire 28
  -495.00,		// x2 wire 29
  -490.00,		// x2 wire 30
  -485.00,		// x2 wire 31
  -480.00,		// x2 wire 32
  -475.00,		// x2 wire 33
  -470.00,		// x2 wire 34
  -465.00,		// x2 wire 35
  -460.00,		// x2 wire 36
  -455.00,		// x2 wire 37
  -450.00,		// x2 wire 38
  -445.00,		// x2 wire 39
  -440.00,		// x2 wire 40
  -435.00,		// x2 wire 41
  -430.00,		// x2 wire 42
  -425.00,		// x2 wire 43
  -420.00,		// x2 wire 44
  -415.00,		// x2 wire 45
  -410.00,		// x2 wire 46
  -405.00,		// x2 wire 47
  -400.00,		// x2 wire 48
  -395.00,		// x2 wire 49
  -390.00,		// x2 wire 50
  -385.00,		// x2 wire 51
  -380.00,		// x2 wire 52
  -375.00,		// x2 wire 53
  -370.00,		// x2 wire 54
  -365.00,		// x2 wire 55
  -360.00,		// x2 wire 56
  -355.00,		// x2 wire 57
  -350.00,		// x2 wire 58
  -345.00,		// x2 wire 59
  -340.00,		// x2 wire 60
  -335.00,		// x2 wire 61
  -330.00,		// x2 wire 62
  -325.00,		// x2 wire 63
  -320.00,		// x2 wire 64
  -315.00,		// x2 wire 65
  -310.00,		// x2 wire 66
  -305.00,		// x2 wire 67
  -300.00,		// x2 wire 68
  -295.00,		// x2 wire 69
  -290.00,		// x2 wire 70
  -285.00,		// x2 wire 71
  -280.00,		// x2 wire 72
  -275.00,		// x2 wire 73
  -270.00,		// x2 wire 74
  -265.00,		// x2 wire 75
  -260.00,		// x2 wire 76
  -255.00,		// x2 wire 77
  -250.00,		// x2 wire 78
  -245.00,		// x2 wire 79
  -240.00,		// x2 wire 80
  -235.00,		// x2 wire 81
  -230.00,		// x2 wire 82
  -225.00,		// x2 wire 83
  -220.00,		// x2 wire 84
  -215.00,		// x2 wire 85
  -210.00,		// x2 wire 86
  -205.00,		// x2 wire 87
  -200.00,		// x2 wire 88
  -195.00,		// x2 wire 89
  -190.00,		// x2 wire 90
  -185.00,		// x2 wire 91
  -180.00,		// x2 wire 92
  -175.00,		// x2 wire 93
  -170.00,		// x2 wire 94
  -165.00,		// x2 wire 95
  -160.00,		// x2 wire 96
  -155.00,		// x2 wire 97
  -150.00,		// x2 wire 98
  -145.00,		// x2 wire 99
  -140.00,		// x2 wire 100
  -135.00,		// x2 wire 101
  -130.00,		// x2 wire 102
  -125.00,		// x2 wire 103
  -120.00,		// x2 wire 104
  -115.00,		// x2 wire 105
  -110.00,		// x2 wire 106
  -105.00,		// x2 wire 107
  -100.00,		// x2 wire 108
   -95.00,		// x2 wire 109
   -90.00,		// x2 wire 110
   -85.00,		// x2 wire 111
   -80.00,		// x2 wire 112
   -75.00,		// x2 wire 113
   -70.00,		// x2 wire 114
   -65.00,		// x2 wire 115
   -60.00,		// x2 wire 116
   -55.00,		// x2 wire 117
   -50.00,		// x2 wire 118
   -45.00,		// x2 wire 119
   -40.00,		// x2 wire 120
   -35.00,		// x2 wire 121
   -30.00,		// x2 wire 122
   -25.00,		// x2 wire 123
   -20.00,		// x2 wire 124
   -15.00,		// x2 wire 125
   -10.00,		// x2 wire 126
    -5.00,		// x2 wire 127
     0.00,		// x2 wire 128
     5.00,		// x2 wire 129
    10.00,		// x2 wire 130
    15.00,		// x2 wire 131
    20.00,		// x2 wire 132
    25.00,		// x2 wire 133
    30.00,		// x2 wire 134
    35.00,		// x2 wire 135
    40.00,		// x2 wire 136
    45.00,		// x2 wire 137
    50.00,		// x2 wire 138
    55.00,		// x2 wire 139
    60.00,		// x2 wire 140
    65.00,		// x2 wire 141
    70.00,		// x2 wire 142
    75.00,		// x2 wire 143
    80.00,		// x2 wire 144
    85.00,		// x2 wire 145
    90.00,		// x2 wire 146
    95.00,		// x2 wire 147
   100.00,		// x2 wire 148
   105.00,		// x2 wire 149
   110.00,		// x2 wire 150
   115.00,		// x2 wire 151
   120.00,		// x2 wire 152
   125.00,		// x2 wire 153
   130.00,		// x2 wire 154
   135.00,		// x2 wire 155
   140.00,		// x2 wire 156
   145.00,		// x2 wire 157
   150.00,		// x2 wire 158
   155.00,		// x2 wire 159
   160.00,		// x2 wire 160
   165.00,		// x2 wire 161
   170.00,		// x2 wire 162
   175.00,		// x2 wire 163
   180.00,		// x2 wire 164
   185.00,		// x2 wire 165
   190.00,		// x2 wire 166
   195.00,		// x2 wire 167
   200.00,		// x2 wire 168
   205.00,		// x2 wire 169
   210.00,		// x2 wire 170
   215.00,		// x2 wire 171
   220.00,		// x2 wire 172
   225.00,		// x2 wire 173
   230.00,		// x2 wire 174
   235.00,		// x2 wire 175
   240.00,		// x2 wire 176
   245.00,		// x2 wire 177
   250.00,		// x2 wire 178
   255.00,		// x2 wire 179
   260.00,		// x2 wire 180
   265.00,		// x2 wire 181
   270.00,		// x2 wire 182
   275.00,		// x2 wire 183
   280.00,		// x2 wire 184
   285.00,		// x2 wire 185
   290.00,		// x2 wire 186
   295.00,		// x2 wire 187
   300.00,		// x2 wire 188
   305.00,		// x2 wire 189
   310.00,		// x2 wire 190
   315.00,		// x2 wire 191
   320.00,		// x2 wire 192
   325.00,		// x2 wire 193
   330.00,		// x2 wire 194
   335.00,		// x2 wire 195
   340.00,		// x2 wire 196
   345.00,		// x2 wire 197
   350.00,		// x2 wire 198
   355.00,		// x2 wire 199
   360.00,		// x2 wire 200
   365.00,		// x2 wire 201
   370.00,		// x2 wire 202
   375.00,		// x2 wire 203
   380.00,		// x2 wire 204
   385.00,		// x2 wire 205
   390.00,		// x2 wire 206
   395.00,		// x2 wire 207
   400.00,		// x2 wire 208
   405.00,		// x2 wire 209
   410.00,		// x2 wire 210
   415.00,		// x2 wire 211
   420.00,		// x2 wire 212
   425.00,		// x2 wire 213
   430.00,		// x2 wire 214
   435.00,		// x2 wire 215
   440.00,		// x2 wire 216
   445.00,		// x2 wire 217
   450.00,		// x2 wire 218
   455.00,		// x2 wire 219
   460.00,		// x2 wire 220
   465.00,		// x2 wire 221
   470.00,		// x2 wire 222
   475.00,		// x2 wire 223
   480.00,		// x2 wire 224
   485.00,		// x2 wire 225
   490.00,		// x2 wire 226
   495.00,		// x2 wire 227
   500.00,		// x2 wire 228
   505.00,		// x2 wire 229
   510.00,		// x2 wire 230
   515.00,		// x2 wire 231
   520.00,		// x2 wire 232
   525.00,		// x2 wire 233
   530.00,		// x2 wire 234
   535.00,		// x2 wire 235
   540.00,		// x2 wire 236
   545.00,		// x2 wire 237
   550.00,		// x2 wire 238
   555.00,		// x2 wire 239
   560.00,		// x2 wire 240
   565.00,		// x2 wire 241
   570.00,		// x2 wire 242
   575.00,		// x2 wire 243
   580.00,		// x2 wire 244
   585.00,		// x2 wire 245
   590.00,		// x2 wire 246
   595.00,		// x2 wire 247
   600.00,		// x2 wire 248
   605.00,		// x2 wire 249
   610.00,		// x2 wire 250
   615.00,		// x2 wire 251
   620.00,		// x2 wire 252
   625.00,		// x2 wire 253
   630.00,		// x2 wire 254
   635.00,		// x2 wire 255
   640.00,		// x2 wire 256
   645.00,		// x2 wire 257
   650.00,		// x2 wire 258
   655.00,		// x2 wire 259
   660.00,		// x2 wire 260
   665.00,		// x2 wire 261
   670.00,		// x2 wire 262
   675.00,		// x2 wire 263
   680.00,		// x2 wire 264
   685.00,		// x2 wire 265
   690.00,		// x2 wire 266
   695.00,		// x2 wire 267
   700.00,		// x2 wire 268
   705.00,		// x2 wire 269
   710.00,		// x2 wire 270
   715.00,		// x2 wire 271
   720.00,		// x2 wire 272
   725.00,		// x2 wire 273
   730.00,		// x2 wire 274
   735.00,		// x2 wire 275
   740.00,		// x2 wire 276
   745.00,		// x2 wire 277
   750.00,		// x2 wire 278
   755.00,		// x2 wire 279
   760.00,		// x2 wire 280
   765.00,		// x2 wire 281
   770.00,		// x2 wire 282
   775.00,		// x2 wire 283
   780.00,		// x2 wire 284
   785.00,		// x2 wire 285
   790.00,		// x2 wire 286
   795.00,		// x2 wire 287
   800.00,		// x2 wire 288
   805.00,		// x2 wire 289
   810.00,		// x2 wire 290
   815.00,		// x2 wire 291
   820.00,		// x2 wire 292
   825.00,		// x2 wire 293
   830.00,		// x2 wire 294
   835.00,		// x2 wire 295
   840.00,		// x2 wire 296
   845.00,		// x2 wire 297
   850.00,		// x2 wire 298
   855.00,		// x2 wire 299
   860.00,		// x2 wire 300
   865.00,		// x2 wire 301
   870.00,		// x2 wire 302
   875.00,		// x2 wire 303
   880.00,		// x2 wire 304
   885.00,		// x2 wire 305
   890.00,		// x2 wire 306
   895.00,		// x2 wire 307
   900.00,		// x2 wire 308
   905.00,		// x2 wire 309
   910.00,		// x2 wire 310
   915.00,		// x2 wire 311
   920.00,		// x2 wire 312
   925.00,		// x2 wire 313
   930.00,		// x2 wire 314
   935.00,		// x2 wire 315
   940.00,		// x2 wire 316
   945.00,		// x2 wire 317
   950.00,		// x2 wire 318
   955.00,		// x2 wire 319
   960.00,		// x2 wire 320
   965.00,		// x2 wire 321
   970.00,		// x2 wire 322
   975.00,		// x2 wire 323
   980.00,		// x2 wire 324
   985.00,		// x2 wire 325
   990.00,		// x2 wire 326
   995.00,		// x2 wire 327
  1000.00,		// x2 wire 328
  1005.00,		// x2 wire 329
  1010.00,		// x2 wire 330
  1015.00,		// x2 wire 331
  1020.00,		// x2 wire 332
  1025.00,		// x2 wire 333
  1030.00,		// x2 wire 334
  1035.00,		// x2 wire 335
  1040.00,		// x2 wire 336
  1045.00,		// x2 wire 337
  1050.00,		// x2 wire 338
  1055.00,		// x2 wire 339
  1060.00,		// x2 wire 340
  1065.00,		// x2 wire 341
  1070.00,		// x2 wire 342
  1075.00,		// x2 wire 343
  1080.00,		// x2 wire 344
  1085.00,		// x2 wire 345
  1090.00,		// x2 wire 346
  1095.00,		// x2 wire 347
  1100.00,		// x2 wire 348
  1105.00,		// x2 wire 349
  1110.00,		// x2 wire 350
  1115.00,		// x2 wire 351
  1120.00,		// x2 wire 352
  1125.00,		// x2 wire 353
  1130.00,		// x2 wire 354
  1135.00,		// x2 wire 355
  1140.00,		// x2 wire 356
  1145.00,		// x2 wire 357
  1150.00,		// x2 wire 358
  1155.00,		// x2 wire 359
  1160.00,		// x2 wire 360
  1165.00,		// x2 wire 361
  1170.00,		// x2 wire 362
  1175.00,		// x2 wire 363
  1180.00,		// x2 wire 364
  1185.00,		// x2 wire 365
  1190.00,		// x2 wire 366
  1195.00,		// x2 wire 367
  1200.00,		// x2 wire 368
  1205.00,		// x2 wire 369
  1210.00,		// x2 wire 370
  1215.00,		// x2 wire 371
  1220.00,		// x2 wire 372
  1225.00,		// x2 wire 373
  1230.00,		// x2 wire 374
  1235.00,		// x2 wire 375
  1240.00,		// x2 wire 376
  1245.00,		// x2 wire 377
  1250.00,		// x2 wire 378
  1255.00,		// x2 wire 379
  1260.00,		// x2 wire 380
  1265.00,		// x2 wire 381
  1270.00,		// x2 wire 382
  1275.00,		// x2 wire 383
  1280.00,		// x2 wire 384
  1285.00,		// x2 wire 385
  1290.00,		// x2 wire 386
  1295.00,		// x2 wire 387
  1300.00,		// x2 wire 388
  1305.00,		// x2 wire 389
  1310.00,		// x2 wire 390
  1315.00,		// x2 wire 391
  1320.00,		// x2 wire 392
  1325.00,		// x2 wire 393
  1330.00,		// x2 wire 394
  1335.00,		// x2 wire 395
  1340.00,		// x2 wire 396
  1345.00,		// x2 wire 397
  1350.00,		// x2 wire 398
  1355.00,		// x2 wire 399
  1360.00		// x2 wire 400
};

static float
position_bs2[CH_B_MAX_S2] = {
  -697.28,		// s2 wire 1
  -692.28,		// s2 wire 2
  -687.28,		// s2 wire 3
  -682.28,		// s2 wire 4
  -677.28,		// s2 wire 5
  -672.28,		// s2 wire 6
  -667.28,		// s2 wire 7
  -662.28,		// s2 wire 8
  -657.28,		// s2 wire 9
  -652.28,		// s2 wire 10
  -647.28,		// s2 wire 11
  -642.28,		// s2 wire 12
  -637.28,		// s2 wire 13
  -632.28,		// s2 wire 14
  -627.28,		// s2 wire 15
  -622.28,		// s2 wire 16
  -617.28,		// s2 wire 17
  -612.28,		// s2 wire 18
  -607.28,		// s2 wire 19
  -602.28,		// s2 wire 20
  -597.28,		// s2 wire 21
  -592.28,		// s2 wire 22
  -587.28,		// s2 wire 23
  -582.28,		// s2 wire 24
  -577.28,		// s2 wire 25
  -572.28,		// s2 wire 26
  -567.28,		// s2 wire 27
  -562.28,		// s2 wire 28
  -557.28,		// s2 wire 29
  -552.28,		// s2 wire 30
  -547.28,		// s2 wire 31
  -542.28,		// s2 wire 32
  -537.28,		// s2 wire 33
  -532.28,		// s2 wire 34
  -527.28,		// s2 wire 35
  -522.28,		// s2 wire 36
  -517.28,		// s2 wire 37
  -512.28,		// s2 wire 38
  -507.28,		// s2 wire 39
  -502.28,		// s2 wire 40
  -497.28,		// s2 wire 41
  -492.28,		// s2 wire 42
  -487.28,		// s2 wire 43
  -482.28,		// s2 wire 44
  -477.28,		// s2 wire 45
  -472.28,		// s2 wire 46
  -467.28,		// s2 wire 47
  -462.28,		// s2 wire 48
  -457.28,		// s2 wire 49
  -452.28,		// s2 wire 50
  -447.28,		// s2 wire 51
  -442.28,		// s2 wire 52
  -437.28,		// s2 wire 53
  -432.28,		// s2 wire 54
  -427.28,		// s2 wire 55
  -422.28,		// s2 wire 56
  -417.28,		// s2 wire 57
  -412.28,		// s2 wire 58
  -407.28,		// s2 wire 59
  -402.28,		// s2 wire 60
  -397.28,		// s2 wire 61
  -392.28,		// s2 wire 62
  -387.28,		// s2 wire 63
  -382.28,		// s2 wire 64
  -377.28,		// s2 wire 65
  -372.28,		// s2 wire 66
  -367.28,		// s2 wire 67
  -362.28,		// s2 wire 68
  -357.28,		// s2 wire 69
  -352.28,		// s2 wire 70
  -347.28,		// s2 wire 71
  -342.28,		// s2 wire 72
  -337.28,		// s2 wire 73
  -332.28,		// s2 wire 74
  -327.28,		// s2 wire 75
  -322.28,		// s2 wire 76
  -317.28,		// s2 wire 77
  -312.28,		// s2 wire 78
  -307.28,		// s2 wire 79
  -302.28,		// s2 wire 80
  -297.28,		// s2 wire 81
  -292.28,		// s2 wire 82
  -287.28,		// s2 wire 83
  -282.28,		// s2 wire 84
  -277.28,		// s2 wire 85
  -272.28,		// s2 wire 86
  -267.28,		// s2 wire 87
  -262.28,		// s2 wire 88
  -257.28,		// s2 wire 89
  -252.28,		// s2 wire 90
  -247.28,		// s2 wire 91
  -242.28,		// s2 wire 92
  -237.28,		// s2 wire 93
  -232.28,		// s2 wire 94
  -227.28,		// s2 wire 95
  -222.28,		// s2 wire 96
  -217.28,		// s2 wire 97
  -212.28,		// s2 wire 98
  -207.28,		// s2 wire 99
  -202.28,		// s2 wire 100
  -197.28,		// s2 wire 101
  -192.28,		// s2 wire 102
  -187.28,		// s2 wire 103
  -182.28,		// s2 wire 104
  -177.28,		// s2 wire 105
  -172.28,		// s2 wire 106
  -167.28,		// s2 wire 107
  -162.28,		// s2 wire 108
  -157.28,		// s2 wire 109
  -152.28,		// s2 wire 110
  -147.28,		// s2 wire 111
  -142.28,		// s2 wire 112
  -137.28,		// s2 wire 113
  -132.28,		// s2 wire 114
  -127.28,		// s2 wire 115
  -122.28,		// s2 wire 116
  -117.28,		// s2 wire 117
  -112.28,		// s2 wire 118
  -107.28,		// s2 wire 119
  -102.28,		// s2 wire 120
   -97.28,		// s2 wire 121
   -92.28,		// s2 wire 122
   -87.28,		// s2 wire 123
   -82.28,		// s2 wire 124
   -77.28,		// s2 wire 125
   -72.28,		// s2 wire 126
   -67.28,		// s2 wire 127
   -62.28,		// s2 wire 128
   -57.28,		// s2 wire 129
   -52.28,		// s2 wire 130
   -47.28,		// s2 wire 131
   -42.28,		// s2 wire 132
   -37.28,		// s2 wire 133
   -32.28,		// s2 wire 134
   -27.28,		// s2 wire 135
   -22.28,		// s2 wire 136
   -17.28,		// s2 wire 137
   -12.28,		// s2 wire 138
    -7.28,		// s2 wire 139
    -2.28,		// s2 wire 140
     2.72,		// s2 wire 141
     7.72,		// s2 wire 142
    12.72,		// s2 wire 143
    17.72,		// s2 wire 144
    22.72,		// s2 wire 145
    27.72,		// s2 wire 146
    32.72,		// s2 wire 147
    37.72,		// s2 wire 148
    42.72,		// s2 wire 149
    47.72,		// s2 wire 150
    52.72,		// s2 wire 151
    57.72,		// s2 wire 152
    62.72,		// s2 wire 153
    67.72,		// s2 wire 154
    72.72,		// s2 wire 155
    77.72,		// s2 wire 156
    82.72,		// s2 wire 157
    87.72,		// s2 wire 158
    92.72,		// s2 wire 159
    97.72,		// s2 wire 160
   102.72,		// s2 wire 161
   107.72,		// s2 wire 162
   112.72,		// s2 wire 163
   117.72,		// s2 wire 164
   122.72,		// s2 wire 165
   127.72,		// s2 wire 166
   132.72,		// s2 wire 167
   137.72,		// s2 wire 168
   142.72,		// s2 wire 169
   147.72,		// s2 wire 170
   152.72,		// s2 wire 171
   157.72,		// s2 wire 172
   162.72,		// s2 wire 173
   167.72,		// s2 wire 174
   172.72,		// s2 wire 175
   177.72,		// s2 wire 176
   182.72,		// s2 wire 177
   187.72,		// s2 wire 178
   192.72,		// s2 wire 179
   197.72,		// s2 wire 180
   202.72,		// s2 wire 181
   207.72,		// s2 wire 182
   212.72,		// s2 wire 183
   217.72,		// s2 wire 184
   222.72,		// s2 wire 185
   227.72,		// s2 wire 186
   232.72,		// s2 wire 187
   237.72,		// s2 wire 188
   242.72,		// s2 wire 189
   247.72,		// s2 wire 190
   252.72,		// s2 wire 191
   257.72,		// s2 wire 192
   262.72,		// s2 wire 193
   267.72,		// s2 wire 194
   272.72,		// s2 wire 195
   277.72,		// s2 wire 196
   282.72,		// s2 wire 197
   287.72,		// s2 wire 198
   292.72,		// s2 wire 199
   297.72,		// s2 wire 200
   302.72,		// s2 wire 201
   307.72,		// s2 wire 202
   312.72,		// s2 wire 203
   317.72,		// s2 wire 204
   322.72,		// s2 wire 205
   327.72,		// s2 wire 206
   332.72,		// s2 wire 207
   337.72,		// s2 wire 208
   342.72,		// s2 wire 209
   347.72,		// s2 wire 210
   352.72,		// s2 wire 211
   357.72,		// s2 wire 212
   362.72,		// s2 wire 213
   367.72,		// s2 wire 214
   372.72,		// s2 wire 215
   377.72,		// s2 wire 216
   382.72,		// s2 wire 217
   387.72,		// s2 wire 218
   392.72,		// s2 wire 219
   397.72,		// s2 wire 220
   402.72,		// s2 wire 221
   407.72,		// s2 wire 222
   412.72,		// s2 wire 223
   417.72,		// s2 wire 224
   422.72,		// s2 wire 225
   427.72,		// s2 wire 226
   432.72,		// s2 wire 227
   437.72,		// s2 wire 228
   442.72,		// s2 wire 229
   447.72,		// s2 wire 230
   452.72,		// s2 wire 231
   457.72,		// s2 wire 232
   462.72,		// s2 wire 233
   467.72,		// s2 wire 234
   472.72,		// s2 wire 235
   477.72,		// s2 wire 236
   482.72,		// s2 wire 237
   487.72,		// s2 wire 238
   492.72,		// s2 wire 239
   497.72,		// s2 wire 240
   502.72,		// s2 wire 241
   507.72,		// s2 wire 242
   512.72,		// s2 wire 243
   517.72,		// s2 wire 244
   522.72,		// s2 wire 245
   527.72,		// s2 wire 246
   532.72,		// s2 wire 247
   537.72,		// s2 wire 248
   542.72,		// s2 wire 249
   547.72,		// s2 wire 250
   552.72,		// s2 wire 251
   557.72,		// s2 wire 252
   562.72,		// s2 wire 253
   567.72,		// s2 wire 254
   572.72,		// s2 wire 255
   577.72,		// s2 wire 256
   582.72,		// s2 wire 257
   587.72,		// s2 wire 258
   592.72,		// s2 wire 259
   597.72,		// s2 wire 260
   602.72,		// s2 wire 261
   607.72,		// s2 wire 262
   612.72,		// s2 wire 263
   617.72,		// s2 wire 264
   622.72,		// s2 wire 265
   627.72,		// s2 wire 266
   632.72,		// s2 wire 267
   637.72,		// s2 wire 268
   642.72,		// s2 wire 269
   647.72,		// s2 wire 270
   652.72,		// s2 wire 271
   657.72,		// s2 wire 272
   662.72,		// s2 wire 273
   667.72,		// s2 wire 274
   672.72,		// s2 wire 275
   677.72,		// s2 wire 276
   682.72,		// s2 wire 277
   687.72,		// s2 wire 278
   692.72,		// s2 wire 279
   697.72,		// s2 wire 280
   702.72,		// s2 wire 281
   707.72,		// s2 wire 282
   712.72,		// s2 wire 283
   717.72,		// s2 wire 284
   722.72,		// s2 wire 285
   727.72,		// s2 wire 286
   732.72,		// s2 wire 287
   737.72,		// s2 wire 288
   742.72,		// s2 wire 289
   747.72,		// s2 wire 290
   752.72,		// s2 wire 291
   757.72,		// s2 wire 292
   762.72,		// s2 wire 293
   767.72,		// s2 wire 294
   772.72,		// s2 wire 295
   777.72,		// s2 wire 296
   782.72,		// s2 wire 297
   787.72,		// s2 wire 298
   792.72,		// s2 wire 299
   797.72,		// s2 wire 300
   802.72,		// s2 wire 301
   807.72,		// s2 wire 302
   812.72,		// s2 wire 303
   817.72,		// s2 wire 304
   822.72,		// s2 wire 305
   827.72,		// s2 wire 306
   832.72,		// s2 wire 307
   837.72,		// s2 wire 308
   842.72,		// s2 wire 309
   847.72,		// s2 wire 310
   852.72,		// s2 wire 311
   857.72,		// s2 wire 312
   862.72,		// s2 wire 313
   867.72,		// s2 wire 314
   872.72,		// s2 wire 315
   877.72,		// s2 wire 316
   882.72,		// s2 wire 317
   887.72,		// s2 wire 318
   892.72,		// s2 wire 319
   897.72,		// s2 wire 320
   902.72,		// s2 wire 321
   907.72,		// s2 wire 322
   912.72,		// s2 wire 323
   917.72,		// s2 wire 324
   922.72,		// s2 wire 325
   927.72,		// s2 wire 326
   932.72,		// s2 wire 327
   937.72,		// s2 wire 328
   942.72,		// s2 wire 329
   947.72,		// s2 wire 330
   952.72,		// s2 wire 331
   957.72,		// s2 wire 332
   962.72,		// s2 wire 333
   967.72,		// s2 wire 334
   972.72,		// s2 wire 335
   977.72,		// s2 wire 336
   982.72,		// s2 wire 337
   987.72,		// s2 wire 338
   992.72,		// s2 wire 339
   997.72,		// s2 wire 340
  1002.72,		// s2 wire 341
  1007.72,		// s2 wire 342
  1012.72,		// s2 wire 343
  1017.72,		// s2 wire 344
  1022.72,		// s2 wire 345
  1027.72,		// s2 wire 346
  1032.72,		// s2 wire 347
  1037.72,		// s2 wire 348
  1042.72,		// s2 wire 349
  1047.72,		// s2 wire 350
  1052.72,		// s2 wire 351
  1057.72,		// s2 wire 352
  1062.72,		// s2 wire 353
  1067.72,		// s2 wire 354
  1072.72,		// s2 wire 355
  1077.72,		// s2 wire 356
  1082.72,		// s2 wire 357
  1087.72,		// s2 wire 358
  1092.72,		// s2 wire 359
  1097.72,		// s2 wire 360
  1102.72,		// s2 wire 361
  1107.72,		// s2 wire 362
  1112.72,		// s2 wire 363
  1117.72,		// s2 wire 364
  1122.72,		// s2 wire 365
  1127.72,		// s2 wire 366
  1132.72,		// s2 wire 367
  1137.72		// s2 wire 368
};

vdcPosOffs vdcPosOffsB = {
  { position_bx1, offset_bx1, CH_B_MAX_X1 },
  { position_bs1, offset_bs1, CH_B_MAX_S1 },
  { position_bx2, offset_bx2, CH_B_MAX_X2 },
  { position_bs2, offset_bs2, CH_B_MAX_S2 }
};

#endif
