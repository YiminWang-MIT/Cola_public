//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chinitA.h,v 2.9 1996-12-12 15:31:33 distler Exp $
//
// INIT file fuer die chamber++ library.
// Offset und Position fuer die Draehte
// von allen vier Ebenen der Driftkammern
// von Spektrometer A.
//

#ifndef __chinitA__
#define __chinitA__

const short CH_A_MAX_X1		= 416;
const short CH_A_MAX_S1		= 352;
const short CH_A_MAX_X2		= 416;
const short CH_A_MAX_S2		= 352;

const short CH_A_OFFSET_X1	= 641;
const short CH_A_OFFSET_S1	= 635;
const short CH_A_OFFSET_X2	= 585;
const short CH_A_OFFSET_S2	= 579;

const double CH_A_ANGLE		= 40.0;
const short  CH_A_FULLRANGE	= 1024;
const double CH_A_TIMETOLENGTH	= 0.5 * 0.05;

static short
offset_ax1[CH_A_MAX_X1] = {
  CH_A_OFFSET_X1,			// wire 1
  CH_A_OFFSET_X1,			// wire 2
  CH_A_OFFSET_X1,			// wire 3
  CH_A_OFFSET_X1,			// wire 4
  CH_A_OFFSET_X1,			// wire 5
  CH_A_OFFSET_X1,			// wire 6
  CH_A_OFFSET_X1,			// wire 7
  CH_A_OFFSET_X1,			// wire 8
  CH_A_OFFSET_X1,			// wire 9
  CH_A_OFFSET_X1,			// wire 10
  CH_A_OFFSET_X1,			// wire 11
  CH_A_OFFSET_X1,			// wire 12
  CH_A_OFFSET_X1,			// wire 13
  CH_A_OFFSET_X1,			// wire 14
  CH_A_OFFSET_X1,			// wire 15
  CH_A_OFFSET_X1,			// wire 16
  CH_A_OFFSET_X1,			// wire 17
  CH_A_OFFSET_X1,			// wire 18
  CH_A_OFFSET_X1,			// wire 19
  CH_A_OFFSET_X1,			// wire 20
  CH_A_OFFSET_X1,			// wire 21
  CH_A_OFFSET_X1,			// wire 22
  CH_A_OFFSET_X1,			// wire 23
  CH_A_OFFSET_X1,			// wire 24
  CH_A_OFFSET_X1,			// wire 25
  CH_A_OFFSET_X1,			// wire 26
  CH_A_OFFSET_X1,			// wire 27
  CH_A_OFFSET_X1,			// wire 28
  CH_A_OFFSET_X1,			// wire 29
  CH_A_OFFSET_X1,			// wire 30
  CH_A_OFFSET_X1,			// wire 31
  CH_A_OFFSET_X1,			// wire 32
  CH_A_OFFSET_X1,			// wire 33
  CH_A_OFFSET_X1,			// wire 34
  CH_A_OFFSET_X1,			// wire 35
  CH_A_OFFSET_X1,			// wire 36
  CH_A_OFFSET_X1,			// wire 37
  CH_A_OFFSET_X1,			// wire 38
  CH_A_OFFSET_X1,			// wire 39
  CH_A_OFFSET_X1,			// wire 40
  CH_A_OFFSET_X1,			// wire 41
  CH_A_OFFSET_X1,			// wire 42
  CH_A_OFFSET_X1,			// wire 43
  CH_A_OFFSET_X1,			// wire 44
  CH_A_OFFSET_X1,			// wire 45
  CH_A_OFFSET_X1,			// wire 46
  CH_A_OFFSET_X1,			// wire 47
  CH_A_OFFSET_X1,			// wire 48
  CH_A_OFFSET_X1,			// wire 49
  CH_A_OFFSET_X1,			// wire 50
  CH_A_OFFSET_X1,			// wire 51
  CH_A_OFFSET_X1,			// wire 52
  CH_A_OFFSET_X1,			// wire 53
  CH_A_OFFSET_X1,			// wire 54
  CH_A_OFFSET_X1,			// wire 55
  CH_A_OFFSET_X1,			// wire 56
  CH_A_OFFSET_X1,			// wire 57
  CH_A_OFFSET_X1,			// wire 58
  CH_A_OFFSET_X1,			// wire 59
  CH_A_OFFSET_X1,			// wire 60
  CH_A_OFFSET_X1,			// wire 61
  CH_A_OFFSET_X1,			// wire 62
  CH_A_OFFSET_X1,			// wire 63
  CH_A_OFFSET_X1,			// wire 64
  CH_A_OFFSET_X1,			// wire 65
  CH_A_OFFSET_X1,			// wire 66
  CH_A_OFFSET_X1,			// wire 67
  CH_A_OFFSET_X1,			// wire 68
  CH_A_OFFSET_X1,			// wire 69
  CH_A_OFFSET_X1,			// wire 70
  CH_A_OFFSET_X1,			// wire 71
  CH_A_OFFSET_X1,			// wire 72
  CH_A_OFFSET_X1,			// wire 73
  CH_A_OFFSET_X1,			// wire 74
  CH_A_OFFSET_X1,			// wire 75
  CH_A_OFFSET_X1,			// wire 76
  CH_A_OFFSET_X1,			// wire 77
  CH_A_OFFSET_X1,			// wire 78
  CH_A_OFFSET_X1,			// wire 79
  CH_A_OFFSET_X1,			// wire 80
  CH_A_OFFSET_X1,			// wire 81
  CH_A_OFFSET_X1,			// wire 82
  CH_A_OFFSET_X1,			// wire 83
  CH_A_OFFSET_X1,			// wire 84
  CH_A_OFFSET_X1,			// wire 85
  CH_A_OFFSET_X1,			// wire 86
  CH_A_OFFSET_X1,			// wire 87
  CH_A_OFFSET_X1,			// wire 88
  CH_A_OFFSET_X1,			// wire 89
  CH_A_OFFSET_X1,			// wire 90
  CH_A_OFFSET_X1,			// wire 91
  CH_A_OFFSET_X1,			// wire 92
  CH_A_OFFSET_X1,			// wire 93
  CH_A_OFFSET_X1,			// wire 94
  CH_A_OFFSET_X1,			// wire 95
  CH_A_OFFSET_X1,			// wire 96
  CH_A_OFFSET_X1,			// wire 97
  CH_A_OFFSET_X1,			// wire 98
  CH_A_OFFSET_X1,			// wire 99
  CH_A_OFFSET_X1,			// wire 100
  CH_A_OFFSET_X1,			// wire 101
  CH_A_OFFSET_X1,			// wire 102
  CH_A_OFFSET_X1,			// wire 103
  CH_A_OFFSET_X1,			// wire 104
  CH_A_OFFSET_X1,			// wire 105
  CH_A_OFFSET_X1,			// wire 106
  CH_A_OFFSET_X1,			// wire 107
  CH_A_OFFSET_X1,			// wire 108
  CH_A_OFFSET_X1,			// wire 109
  CH_A_OFFSET_X1,			// wire 110
  CH_A_OFFSET_X1,			// wire 111
  CH_A_OFFSET_X1,			// wire 112
  CH_A_OFFSET_X1,			// wire 113
  CH_A_OFFSET_X1,			// wire 114
  CH_A_OFFSET_X1,			// wire 115
  CH_A_OFFSET_X1,			// wire 116
  CH_A_OFFSET_X1,			// wire 117
  CH_A_OFFSET_X1,			// wire 118
  CH_A_OFFSET_X1,			// wire 119
  CH_A_OFFSET_X1,			// wire 120
  CH_A_OFFSET_X1,			// wire 121
  CH_A_OFFSET_X1,			// wire 122
  CH_A_OFFSET_X1,			// wire 123
  CH_A_OFFSET_X1,			// wire 124
  CH_A_OFFSET_X1,			// wire 125
  CH_A_OFFSET_X1,			// wire 126
  CH_A_OFFSET_X1,			// wire 127
  CH_A_OFFSET_X1,			// wire 128
  CH_A_OFFSET_X1,			// wire 129
  CH_A_OFFSET_X1,			// wire 130
  CH_A_OFFSET_X1,			// wire 131
  CH_A_OFFSET_X1,			// wire 132
  CH_A_OFFSET_X1,			// wire 133
  CH_A_OFFSET_X1,			// wire 134
  CH_A_OFFSET_X1,			// wire 135
  CH_A_OFFSET_X1,			// wire 136
  CH_A_OFFSET_X1,			// wire 137
  CH_A_OFFSET_X1,			// wire 138
  CH_A_OFFSET_X1,			// wire 139
  CH_A_OFFSET_X1,			// wire 140
  CH_A_OFFSET_X1,			// wire 141
  CH_A_OFFSET_X1,			// wire 142
  CH_A_OFFSET_X1,			// wire 143
  CH_A_OFFSET_X1,			// wire 144
  CH_A_OFFSET_X1,			// wire 145
  CH_A_OFFSET_X1,			// wire 146
  CH_A_OFFSET_X1,			// wire 147
  CH_A_OFFSET_X1,			// wire 148
  CH_A_OFFSET_X1,			// wire 149
  CH_A_OFFSET_X1,			// wire 150
  CH_A_OFFSET_X1,			// wire 151
  CH_A_OFFSET_X1,			// wire 152
  CH_A_OFFSET_X1,			// wire 153
  CH_A_OFFSET_X1,			// wire 154
  CH_A_OFFSET_X1,			// wire 155
  CH_A_OFFSET_X1,			// wire 156
  CH_A_OFFSET_X1,			// wire 157
  CH_A_OFFSET_X1,			// wire 158
  CH_A_OFFSET_X1,			// wire 159
  CH_A_OFFSET_X1,			// wire 160
  CH_A_OFFSET_X1,			// wire 161
  CH_A_OFFSET_X1,			// wire 162
  CH_A_OFFSET_X1,			// wire 163
  CH_A_OFFSET_X1,			// wire 164
  CH_A_OFFSET_X1,			// wire 165
  CH_A_OFFSET_X1,			// wire 166
  CH_A_OFFSET_X1,			// wire 167
  CH_A_OFFSET_X1,			// wire 168
  CH_A_OFFSET_X1,			// wire 169
  CH_A_OFFSET_X1,			// wire 170
  CH_A_OFFSET_X1,			// wire 171
  CH_A_OFFSET_X1,			// wire 172
  CH_A_OFFSET_X1,			// wire 173
  CH_A_OFFSET_X1,			// wire 174
  CH_A_OFFSET_X1,			// wire 175
  CH_A_OFFSET_X1,			// wire 176
  CH_A_OFFSET_X1,			// wire 177
  CH_A_OFFSET_X1,			// wire 178
  CH_A_OFFSET_X1,			// wire 179
  CH_A_OFFSET_X1,			// wire 180
  CH_A_OFFSET_X1,			// wire 181
  CH_A_OFFSET_X1,			// wire 182
  CH_A_OFFSET_X1,			// wire 183
  CH_A_OFFSET_X1,			// wire 184
  CH_A_OFFSET_X1,			// wire 185
  CH_A_OFFSET_X1,			// wire 186
  CH_A_OFFSET_X1,			// wire 187
  CH_A_OFFSET_X1,			// wire 188
  CH_A_OFFSET_X1,			// wire 189
  CH_A_OFFSET_X1,			// wire 190
  CH_A_OFFSET_X1,			// wire 191
  CH_A_OFFSET_X1,			// wire 192
  CH_A_OFFSET_X1,			// wire 193
  CH_A_OFFSET_X1,			// wire 194
  CH_A_OFFSET_X1,			// wire 195
  CH_A_OFFSET_X1,			// wire 196
  CH_A_OFFSET_X1,			// wire 197
  CH_A_OFFSET_X1,			// wire 198
  CH_A_OFFSET_X1,			// wire 199
  CH_A_OFFSET_X1,			// wire 200
  CH_A_OFFSET_X1,			// wire 201
  CH_A_OFFSET_X1,			// wire 202
  CH_A_OFFSET_X1,			// wire 203
  CH_A_OFFSET_X1,			// wire 204
  CH_A_OFFSET_X1,			// wire 205
  CH_A_OFFSET_X1,			// wire 206
  CH_A_OFFSET_X1,			// wire 207
  CH_A_OFFSET_X1,			// wire 208
  CH_A_OFFSET_X1,			// wire 209
  CH_A_OFFSET_X1,			// wire 210
  CH_A_OFFSET_X1,			// wire 211
  CH_A_OFFSET_X1,			// wire 212
  CH_A_OFFSET_X1,			// wire 213
  CH_A_OFFSET_X1,			// wire 214
  CH_A_OFFSET_X1,			// wire 215
  CH_A_OFFSET_X1,			// wire 216
  CH_A_OFFSET_X1,			// wire 217
  CH_A_OFFSET_X1,			// wire 218
  CH_A_OFFSET_X1,			// wire 219
  CH_A_OFFSET_X1,			// wire 220
  CH_A_OFFSET_X1,			// wire 221
  CH_A_OFFSET_X1,			// wire 222
  CH_A_OFFSET_X1,			// wire 223
  CH_A_OFFSET_X1,			// wire 224
  CH_A_OFFSET_X1,			// wire 225
  CH_A_OFFSET_X1,			// wire 226
  CH_A_OFFSET_X1,			// wire 227
  CH_A_OFFSET_X1,			// wire 228
  CH_A_OFFSET_X1,			// wire 229
  CH_A_OFFSET_X1,			// wire 230
  CH_A_OFFSET_X1,			// wire 231
  CH_A_OFFSET_X1,			// wire 232
  CH_A_OFFSET_X1,			// wire 233
  CH_A_OFFSET_X1,			// wire 234
  CH_A_OFFSET_X1,			// wire 235
  CH_A_OFFSET_X1,			// wire 236
  CH_A_OFFSET_X1,			// wire 237
  CH_A_OFFSET_X1,			// wire 238
  CH_A_OFFSET_X1,			// wire 239
  CH_A_OFFSET_X1,			// wire 240
  CH_A_OFFSET_X1,			// wire 241
  CH_A_OFFSET_X1,			// wire 242
  CH_A_OFFSET_X1,			// wire 243
  CH_A_OFFSET_X1,			// wire 244
  CH_A_OFFSET_X1,			// wire 245
  CH_A_OFFSET_X1,			// wire 246
  CH_A_OFFSET_X1,			// wire 247
  CH_A_OFFSET_X1,			// wire 248
  CH_A_OFFSET_X1,			// wire 249
  CH_A_OFFSET_X1,			// wire 250
  CH_A_OFFSET_X1,			// wire 251
  CH_A_OFFSET_X1,			// wire 252
  CH_A_OFFSET_X1,			// wire 253
  CH_A_OFFSET_X1,			// wire 254
  CH_A_OFFSET_X1,			// wire 255
  CH_A_OFFSET_X1,			// wire 256
  CH_A_OFFSET_X1,			// wire 257
  CH_A_OFFSET_X1,			// wire 258
  CH_A_OFFSET_X1,			// wire 259
  CH_A_OFFSET_X1,			// wire 260
  CH_A_OFFSET_X1,			// wire 261
  CH_A_OFFSET_X1,			// wire 262
  CH_A_OFFSET_X1,			// wire 263
  CH_A_OFFSET_X1,			// wire 264
  CH_A_OFFSET_X1,			// wire 265
  CH_A_OFFSET_X1,			// wire 266
  CH_A_OFFSET_X1,			// wire 267
  CH_A_OFFSET_X1,			// wire 268
  CH_A_OFFSET_X1,			// wire 269
  CH_A_OFFSET_X1,			// wire 270
  CH_A_OFFSET_X1,			// wire 271
  CH_A_OFFSET_X1,			// wire 272
  CH_A_OFFSET_X1,			// wire 273
  CH_A_OFFSET_X1,			// wire 274
  CH_A_OFFSET_X1,			// wire 275
  CH_A_OFFSET_X1,			// wire 276
  CH_A_OFFSET_X1,			// wire 277
  CH_A_OFFSET_X1,			// wire 278
  CH_A_OFFSET_X1,			// wire 279
  CH_A_OFFSET_X1,			// wire 280
  CH_A_OFFSET_X1,			// wire 281
  CH_A_OFFSET_X1,			// wire 282
  CH_A_OFFSET_X1,			// wire 283
  CH_A_OFFSET_X1,			// wire 284
  CH_A_OFFSET_X1,			// wire 285
  CH_A_OFFSET_X1,			// wire 286
  CH_A_OFFSET_X1,			// wire 287
  CH_A_OFFSET_X1,			// wire 288
  CH_A_OFFSET_X1,			// wire 289
  CH_A_OFFSET_X1,			// wire 290
  CH_A_OFFSET_X1,			// wire 291
  CH_A_OFFSET_X1,			// wire 292
  CH_A_OFFSET_X1,			// wire 293
  CH_A_OFFSET_X1,			// wire 294
  CH_A_OFFSET_X1,			// wire 295
  CH_A_OFFSET_X1,			// wire 296
  CH_A_OFFSET_X1,			// wire 297
  CH_A_OFFSET_X1,			// wire 298
  CH_A_OFFSET_X1,			// wire 299
  CH_A_OFFSET_X1,			// wire 300
  CH_A_OFFSET_X1,			// wire 301
  CH_A_OFFSET_X1,			// wire 302
  CH_A_OFFSET_X1,			// wire 303
  CH_A_OFFSET_X1,			// wire 304
  CH_A_OFFSET_X1,			// wire 305
  CH_A_OFFSET_X1,			// wire 306
  CH_A_OFFSET_X1,			// wire 307
  CH_A_OFFSET_X1,			// wire 308
  CH_A_OFFSET_X1,			// wire 309
  CH_A_OFFSET_X1,			// wire 310
  CH_A_OFFSET_X1,			// wire 311
  CH_A_OFFSET_X1,			// wire 312
  CH_A_OFFSET_X1,			// wire 313
  CH_A_OFFSET_X1,			// wire 314
  CH_A_OFFSET_X1,			// wire 315
  CH_A_OFFSET_X1,			// wire 316
  CH_A_OFFSET_X1,			// wire 317
  CH_A_OFFSET_X1,			// wire 318
  CH_A_OFFSET_X1,			// wire 319
  CH_A_OFFSET_X1,			// wire 320
  CH_A_OFFSET_X1,			// wire 321
  CH_A_OFFSET_X1,			// wire 322
  CH_A_OFFSET_X1,			// wire 323
  CH_A_OFFSET_X1,			// wire 324
  CH_A_OFFSET_X1,			// wire 325
  CH_A_OFFSET_X1,			// wire 326
  CH_A_OFFSET_X1,			// wire 327
  CH_A_OFFSET_X1,			// wire 328
  CH_A_OFFSET_X1,			// wire 329
  CH_A_OFFSET_X1,			// wire 330
  CH_A_OFFSET_X1,			// wire 331
  CH_A_OFFSET_X1,			// wire 332
  CH_A_OFFSET_X1,			// wire 333
  CH_A_OFFSET_X1,			// wire 334
  CH_A_OFFSET_X1,			// wire 335
  CH_A_OFFSET_X1,			// wire 336
  CH_A_OFFSET_X1,			// wire 337
  CH_A_OFFSET_X1,			// wire 338
  CH_A_OFFSET_X1,			// wire 339
  CH_A_OFFSET_X1,			// wire 340
  CH_A_OFFSET_X1,			// wire 341
  CH_A_OFFSET_X1,			// wire 342
  CH_A_OFFSET_X1,			// wire 343
  CH_A_OFFSET_X1,			// wire 344
  CH_A_OFFSET_X1,			// wire 345
  CH_A_OFFSET_X1,			// wire 346
  CH_A_OFFSET_X1,			// wire 347
  CH_A_OFFSET_X1,			// wire 348
  CH_A_OFFSET_X1,			// wire 349
  CH_A_OFFSET_X1,			// wire 350
  CH_A_OFFSET_X1,			// wire 351
  CH_A_OFFSET_X1,			// wire 352
  CH_A_OFFSET_X1,			// wire 353
  CH_A_OFFSET_X1,			// wire 354
  CH_A_OFFSET_X1,			// wire 355
  CH_A_OFFSET_X1,			// wire 356
  CH_A_OFFSET_X1,			// wire 357
  CH_A_OFFSET_X1,			// wire 358
  CH_A_OFFSET_X1,			// wire 359
  CH_A_OFFSET_X1,			// wire 360
  CH_A_OFFSET_X1,			// wire 361
  CH_A_OFFSET_X1,			// wire 362
  CH_A_OFFSET_X1,			// wire 363
  CH_A_OFFSET_X1,			// wire 364
  CH_A_OFFSET_X1,			// wire 365
  CH_A_OFFSET_X1,			// wire 366
  CH_A_OFFSET_X1,			// wire 367
  CH_A_OFFSET_X1,			// wire 368
  CH_A_OFFSET_X1,			// wire 369
  CH_A_OFFSET_X1,			// wire 370
  CH_A_OFFSET_X1,			// wire 371
  CH_A_OFFSET_X1,			// wire 372
  CH_A_OFFSET_X1,			// wire 373
  CH_A_OFFSET_X1,			// wire 374
  CH_A_OFFSET_X1,			// wire 375
  CH_A_OFFSET_X1,			// wire 376
  CH_A_OFFSET_X1,			// wire 377
  CH_A_OFFSET_X1,			// wire 378
  CH_A_OFFSET_X1,			// wire 379
  CH_A_OFFSET_X1,			// wire 380
  CH_A_OFFSET_X1,			// wire 381
  CH_A_OFFSET_X1,			// wire 382
  CH_A_OFFSET_X1,			// wire 383
  CH_A_OFFSET_X1,			// wire 384
  CH_A_OFFSET_X1,			// wire 385
  CH_A_OFFSET_X1,			// wire 386
  CH_A_OFFSET_X1,			// wire 387
  CH_A_OFFSET_X1,			// wire 388
  CH_A_OFFSET_X1,			// wire 389
  CH_A_OFFSET_X1,			// wire 390
  CH_A_OFFSET_X1,			// wire 391
  CH_A_OFFSET_X1,			// wire 392
  CH_A_OFFSET_X1,			// wire 393
  CH_A_OFFSET_X1,			// wire 394
  CH_A_OFFSET_X1,			// wire 395
  CH_A_OFFSET_X1,			// wire 396
  CH_A_OFFSET_X1,			// wire 397
  CH_A_OFFSET_X1,			// wire 398
  CH_A_OFFSET_X1,			// wire 399
  CH_A_OFFSET_X1,			// wire 400
  CH_A_OFFSET_X1,			// wire 401
  CH_A_OFFSET_X1,			// wire 402
  CH_A_OFFSET_X1,			// wire 403
  CH_A_OFFSET_X1,			// wire 404
  CH_A_OFFSET_X1,			// wire 405
  CH_A_OFFSET_X1,			// wire 406
  CH_A_OFFSET_X1,			// wire 407
  CH_A_OFFSET_X1,			// wire 408
  CH_A_OFFSET_X1,			// wire 409
  CH_A_OFFSET_X1,			// wire 410
  CH_A_OFFSET_X1,			// wire 411
  CH_A_OFFSET_X1,			// wire 412
  CH_A_OFFSET_X1,			// wire 413
  CH_A_OFFSET_X1,			// wire 414
  CH_A_OFFSET_X1,			// wire 415
  CH_A_OFFSET_X1			// wire 416
};

static short
offset_as1[CH_A_MAX_S1] = {
  CH_A_OFFSET_S1,			// wire 1
  CH_A_OFFSET_S1,			// wire 2
  CH_A_OFFSET_S1,			// wire 3
  CH_A_OFFSET_S1,			// wire 4
  CH_A_OFFSET_S1,			// wire 5
  CH_A_OFFSET_S1,			// wire 6
  CH_A_OFFSET_S1,			// wire 7
  CH_A_OFFSET_S1,			// wire 8
  CH_A_OFFSET_S1,			// wire 9
  CH_A_OFFSET_S1,			// wire 10
  CH_A_OFFSET_S1,			// wire 11
  CH_A_OFFSET_S1,			// wire 12
  CH_A_OFFSET_S1,			// wire 13
  CH_A_OFFSET_S1,			// wire 14
  CH_A_OFFSET_S1,			// wire 15
  CH_A_OFFSET_S1,			// wire 16
  CH_A_OFFSET_S1,			// wire 17
  CH_A_OFFSET_S1,			// wire 18
  CH_A_OFFSET_S1,			// wire 19
  CH_A_OFFSET_S1,			// wire 20
  CH_A_OFFSET_S1,			// wire 21
  CH_A_OFFSET_S1,			// wire 22
  CH_A_OFFSET_S1,			// wire 23
  CH_A_OFFSET_S1,			// wire 24
  CH_A_OFFSET_S1,			// wire 25
  CH_A_OFFSET_S1,			// wire 26
  CH_A_OFFSET_S1,			// wire 27
  CH_A_OFFSET_S1,			// wire 28
  CH_A_OFFSET_S1,			// wire 29
  CH_A_OFFSET_S1,			// wire 30
  CH_A_OFFSET_S1,			// wire 31
  CH_A_OFFSET_S1,			// wire 32
  CH_A_OFFSET_S1,			// wire 33
  CH_A_OFFSET_S1,			// wire 34
  CH_A_OFFSET_S1,			// wire 35
  CH_A_OFFSET_S1,			// wire 36
  CH_A_OFFSET_S1,			// wire 37
  CH_A_OFFSET_S1,			// wire 38
  CH_A_OFFSET_S1,			// wire 39
  CH_A_OFFSET_S1,			// wire 40
  CH_A_OFFSET_S1,			// wire 41
  CH_A_OFFSET_S1,			// wire 42
  CH_A_OFFSET_S1,			// wire 43
  CH_A_OFFSET_S1,			// wire 44
  CH_A_OFFSET_S1,			// wire 45
  CH_A_OFFSET_S1,			// wire 46
  CH_A_OFFSET_S1,			// wire 47
  CH_A_OFFSET_S1,			// wire 48
  CH_A_OFFSET_S1,			// wire 49
  CH_A_OFFSET_S1,			// wire 50
  CH_A_OFFSET_S1,			// wire 51
  CH_A_OFFSET_S1,			// wire 52
  CH_A_OFFSET_S1,			// wire 53
  CH_A_OFFSET_S1,			// wire 54
  CH_A_OFFSET_S1,			// wire 55
  CH_A_OFFSET_S1,			// wire 56
  CH_A_OFFSET_S1,			// wire 57
  CH_A_OFFSET_S1,			// wire 58
  CH_A_OFFSET_S1,			// wire 59
  CH_A_OFFSET_S1,			// wire 60
  CH_A_OFFSET_S1,			// wire 61
  CH_A_OFFSET_S1,			// wire 62
  CH_A_OFFSET_S1,			// wire 63
  CH_A_OFFSET_S1,			// wire 64
  CH_A_OFFSET_S1,			// wire 65
  CH_A_OFFSET_S1,			// wire 66
  CH_A_OFFSET_S1,			// wire 67
  CH_A_OFFSET_S1,			// wire 68
  CH_A_OFFSET_S1,			// wire 69
  CH_A_OFFSET_S1,			// wire 70
  CH_A_OFFSET_S1,			// wire 71
  CH_A_OFFSET_S1,			// wire 72
  CH_A_OFFSET_S1,			// wire 73
  CH_A_OFFSET_S1,			// wire 74
  CH_A_OFFSET_S1,			// wire 75
  CH_A_OFFSET_S1,			// wire 76
  CH_A_OFFSET_S1,			// wire 77
  CH_A_OFFSET_S1,			// wire 78
  CH_A_OFFSET_S1,			// wire 79
  CH_A_OFFSET_S1,			// wire 80
  CH_A_OFFSET_S1,			// wire 81
  CH_A_OFFSET_S1,			// wire 82
  CH_A_OFFSET_S1,			// wire 83
  CH_A_OFFSET_S1,			// wire 84
  CH_A_OFFSET_S1,			// wire 85
  CH_A_OFFSET_S1,			// wire 86
  CH_A_OFFSET_S1,			// wire 87
  CH_A_OFFSET_S1,			// wire 88
  CH_A_OFFSET_S1,			// wire 89
  CH_A_OFFSET_S1,			// wire 90
  CH_A_OFFSET_S1,			// wire 91
  CH_A_OFFSET_S1,			// wire 92
  CH_A_OFFSET_S1,			// wire 93
  CH_A_OFFSET_S1,			// wire 94
  CH_A_OFFSET_S1,			// wire 95
  CH_A_OFFSET_S1,			// wire 96
  CH_A_OFFSET_S1,			// wire 97
  CH_A_OFFSET_S1,			// wire 98
  CH_A_OFFSET_S1,			// wire 99
  CH_A_OFFSET_S1,			// wire 100
  CH_A_OFFSET_S1,			// wire 101
  CH_A_OFFSET_S1,			// wire 102
  CH_A_OFFSET_S1,			// wire 103
  CH_A_OFFSET_S1,			// wire 104
  CH_A_OFFSET_S1,			// wire 105
  CH_A_OFFSET_S1,			// wire 106
  CH_A_OFFSET_S1,			// wire 107
  CH_A_OFFSET_S1,			// wire 108
  CH_A_OFFSET_S1,			// wire 109
  CH_A_OFFSET_S1,			// wire 110
  CH_A_OFFSET_S1,			// wire 111
  CH_A_OFFSET_S1,			// wire 112
  CH_A_OFFSET_S1,			// wire 113
  CH_A_OFFSET_S1,			// wire 114
  CH_A_OFFSET_S1,			// wire 115
  CH_A_OFFSET_S1,			// wire 116
  CH_A_OFFSET_S1,			// wire 117
  CH_A_OFFSET_S1,			// wire 118
  CH_A_OFFSET_S1,			// wire 119
  CH_A_OFFSET_S1,			// wire 120
  CH_A_OFFSET_S1,			// wire 121
  CH_A_OFFSET_S1,			// wire 122
  CH_A_OFFSET_S1,			// wire 123
  CH_A_OFFSET_S1,			// wire 124
  CH_A_OFFSET_S1,			// wire 125
  CH_A_OFFSET_S1,			// wire 126
  CH_A_OFFSET_S1,			// wire 127
  CH_A_OFFSET_S1,			// wire 128
  CH_A_OFFSET_S1,			// wire 129
  CH_A_OFFSET_S1,			// wire 130
  CH_A_OFFSET_S1,			// wire 131
  CH_A_OFFSET_S1,			// wire 132
  CH_A_OFFSET_S1,			// wire 133
  CH_A_OFFSET_S1,			// wire 134
  CH_A_OFFSET_S1,			// wire 135
  CH_A_OFFSET_S1,			// wire 136
  CH_A_OFFSET_S1,			// wire 137
  CH_A_OFFSET_S1,			// wire 138
  CH_A_OFFSET_S1,			// wire 139
  CH_A_OFFSET_S1,			// wire 140
  CH_A_OFFSET_S1,			// wire 141
  CH_A_OFFSET_S1,			// wire 142
  CH_A_OFFSET_S1,			// wire 143
  CH_A_OFFSET_S1,			// wire 144
  CH_A_OFFSET_S1,			// wire 145
  CH_A_OFFSET_S1,			// wire 146
  CH_A_OFFSET_S1,			// wire 147
  CH_A_OFFSET_S1,			// wire 148
  CH_A_OFFSET_S1,			// wire 149
  CH_A_OFFSET_S1,			// wire 150
  CH_A_OFFSET_S1,			// wire 151
  CH_A_OFFSET_S1,			// wire 152
  CH_A_OFFSET_S1,			// wire 153
  CH_A_OFFSET_S1,			// wire 154
  CH_A_OFFSET_S1,			// wire 155
  CH_A_OFFSET_S1,			// wire 156
  CH_A_OFFSET_S1,			// wire 157
  CH_A_OFFSET_S1,			// wire 158
  CH_A_OFFSET_S1,			// wire 159
  CH_A_OFFSET_S1,			// wire 160
  CH_A_OFFSET_S1,			// wire 161
  CH_A_OFFSET_S1,			// wire 162
  CH_A_OFFSET_S1,			// wire 163
  CH_A_OFFSET_S1,			// wire 164
  CH_A_OFFSET_S1,			// wire 165
  CH_A_OFFSET_S1,			// wire 166
  CH_A_OFFSET_S1,			// wire 167
  CH_A_OFFSET_S1,			// wire 168
  CH_A_OFFSET_S1,			// wire 169
  CH_A_OFFSET_S1,			// wire 170
  CH_A_OFFSET_S1,			// wire 171
  CH_A_OFFSET_S1,			// wire 172
  CH_A_OFFSET_S1,			// wire 173
  CH_A_OFFSET_S1,			// wire 174
  CH_A_OFFSET_S1,			// wire 175
  CH_A_OFFSET_S1,			// wire 176
  CH_A_OFFSET_S1,			// wire 177
  CH_A_OFFSET_S1,			// wire 178
  CH_A_OFFSET_S1,			// wire 179
  CH_A_OFFSET_S1,			// wire 180
  CH_A_OFFSET_S1,			// wire 181
  CH_A_OFFSET_S1,			// wire 182
  CH_A_OFFSET_S1,			// wire 183
  CH_A_OFFSET_S1,			// wire 184
  CH_A_OFFSET_S1,			// wire 185
  CH_A_OFFSET_S1,			// wire 186
  CH_A_OFFSET_S1,			// wire 187
  CH_A_OFFSET_S1,			// wire 188
  CH_A_OFFSET_S1,			// wire 189
  CH_A_OFFSET_S1,			// wire 190
  CH_A_OFFSET_S1,			// wire 191
  CH_A_OFFSET_S1,			// wire 192
  CH_A_OFFSET_S1,			// wire 193
  CH_A_OFFSET_S1,			// wire 194
  CH_A_OFFSET_S1,			// wire 195
  CH_A_OFFSET_S1,			// wire 196
  CH_A_OFFSET_S1,			// wire 197
  CH_A_OFFSET_S1,			// wire 198
  CH_A_OFFSET_S1,			// wire 199
  CH_A_OFFSET_S1,			// wire 200
  CH_A_OFFSET_S1,			// wire 201
  CH_A_OFFSET_S1,			// wire 202
  CH_A_OFFSET_S1,			// wire 203
  CH_A_OFFSET_S1,			// wire 204
  CH_A_OFFSET_S1,			// wire 205
  CH_A_OFFSET_S1,			// wire 206
  CH_A_OFFSET_S1,			// wire 207
  CH_A_OFFSET_S1,			// wire 208
  CH_A_OFFSET_S1,			// wire 209
  CH_A_OFFSET_S1,			// wire 210
  CH_A_OFFSET_S1,			// wire 211
  CH_A_OFFSET_S1,			// wire 212
  CH_A_OFFSET_S1,			// wire 213
  CH_A_OFFSET_S1,			// wire 214
  CH_A_OFFSET_S1,			// wire 215
  CH_A_OFFSET_S1,			// wire 216
  CH_A_OFFSET_S1,			// wire 217
  CH_A_OFFSET_S1,			// wire 218
  CH_A_OFFSET_S1,			// wire 219
  CH_A_OFFSET_S1,			// wire 220
  CH_A_OFFSET_S1,			// wire 221
  CH_A_OFFSET_S1,			// wire 222
  CH_A_OFFSET_S1,			// wire 223
  CH_A_OFFSET_S1,			// wire 224
  CH_A_OFFSET_S1,			// wire 225
  CH_A_OFFSET_S1,			// wire 226
  CH_A_OFFSET_S1,			// wire 227
  CH_A_OFFSET_S1,			// wire 228
  CH_A_OFFSET_S1,			// wire 229
  CH_A_OFFSET_S1,			// wire 230
  CH_A_OFFSET_S1,			// wire 231
  CH_A_OFFSET_S1,			// wire 232
  CH_A_OFFSET_S1,			// wire 233
  CH_A_OFFSET_S1,			// wire 234
  CH_A_OFFSET_S1,			// wire 235
  CH_A_OFFSET_S1,			// wire 236
  CH_A_OFFSET_S1,			// wire 237
  CH_A_OFFSET_S1,			// wire 238
  CH_A_OFFSET_S1,			// wire 239
  CH_A_OFFSET_S1,			// wire 240
  CH_A_OFFSET_S1,			// wire 241
  CH_A_OFFSET_S1,			// wire 242
  CH_A_OFFSET_S1,			// wire 243
  CH_A_OFFSET_S1,			// wire 244
  CH_A_OFFSET_S1,			// wire 245
  CH_A_OFFSET_S1,			// wire 246
  CH_A_OFFSET_S1,			// wire 247
  CH_A_OFFSET_S1,			// wire 248
  CH_A_OFFSET_S1,			// wire 249
  CH_A_OFFSET_S1,			// wire 250
  CH_A_OFFSET_S1,			// wire 251
  CH_A_OFFSET_S1,			// wire 252
  CH_A_OFFSET_S1,			// wire 253
  CH_A_OFFSET_S1,			// wire 254
  CH_A_OFFSET_S1,			// wire 255
  CH_A_OFFSET_S1,			// wire 256
  CH_A_OFFSET_S1,			// wire 257
  CH_A_OFFSET_S1,			// wire 258
  CH_A_OFFSET_S1,			// wire 259
  CH_A_OFFSET_S1,			// wire 260
  CH_A_OFFSET_S1,			// wire 261
  CH_A_OFFSET_S1,			// wire 262
  CH_A_OFFSET_S1,			// wire 263
  CH_A_OFFSET_S1,			// wire 264
  CH_A_OFFSET_S1,			// wire 265
  CH_A_OFFSET_S1,			// wire 266
  CH_A_OFFSET_S1,			// wire 267
  CH_A_OFFSET_S1,			// wire 268
  CH_A_OFFSET_S1,			// wire 269
  CH_A_OFFSET_S1,			// wire 270
  CH_A_OFFSET_S1,			// wire 271
  CH_A_OFFSET_S1,			// wire 272
  CH_A_OFFSET_S1,			// wire 273
  CH_A_OFFSET_S1,			// wire 274
  CH_A_OFFSET_S1,			// wire 275
  CH_A_OFFSET_S1,			// wire 276
  CH_A_OFFSET_S1,			// wire 277
  CH_A_OFFSET_S1,			// wire 278
  CH_A_OFFSET_S1,			// wire 279
  CH_A_OFFSET_S1,			// wire 280
  CH_A_OFFSET_S1,			// wire 281
  CH_A_OFFSET_S1,			// wire 282
  CH_A_OFFSET_S1,			// wire 283
  CH_A_OFFSET_S1,			// wire 284
  CH_A_OFFSET_S1,			// wire 285
  CH_A_OFFSET_S1,			// wire 286
  CH_A_OFFSET_S1,			// wire 287
  CH_A_OFFSET_S1,			// wire 288
  CH_A_OFFSET_S1,			// wire 289
  CH_A_OFFSET_S1,			// wire 290
  CH_A_OFFSET_S1,			// wire 291
  CH_A_OFFSET_S1,			// wire 292
  CH_A_OFFSET_S1,			// wire 293
  CH_A_OFFSET_S1,			// wire 294
  CH_A_OFFSET_S1,			// wire 295
  CH_A_OFFSET_S1,			// wire 296
  CH_A_OFFSET_S1,			// wire 297
  CH_A_OFFSET_S1,			// wire 298
  CH_A_OFFSET_S1,			// wire 299
  CH_A_OFFSET_S1,			// wire 300
  CH_A_OFFSET_S1,			// wire 301
  CH_A_OFFSET_S1,			// wire 302
  CH_A_OFFSET_S1,			// wire 303
  CH_A_OFFSET_S1,			// wire 304
  CH_A_OFFSET_S1,			// wire 305
  CH_A_OFFSET_S1,			// wire 306
  CH_A_OFFSET_S1,			// wire 307
  CH_A_OFFSET_S1,			// wire 308
  CH_A_OFFSET_S1,			// wire 309
  CH_A_OFFSET_S1,			// wire 310
  CH_A_OFFSET_S1,			// wire 311
  CH_A_OFFSET_S1,			// wire 312
  CH_A_OFFSET_S1,			// wire 313
  CH_A_OFFSET_S1,			// wire 314
  CH_A_OFFSET_S1,			// wire 315
  CH_A_OFFSET_S1,			// wire 316
  CH_A_OFFSET_S1,			// wire 317
  CH_A_OFFSET_S1,			// wire 318
  CH_A_OFFSET_S1,			// wire 319
  CH_A_OFFSET_S1,			// wire 320
  CH_A_OFFSET_S1,			// wire 321
  CH_A_OFFSET_S1,			// wire 322
  CH_A_OFFSET_S1,			// wire 323
  CH_A_OFFSET_S1,			// wire 324
  CH_A_OFFSET_S1,			// wire 325
  CH_A_OFFSET_S1,			// wire 326
  CH_A_OFFSET_S1,			// wire 327
  CH_A_OFFSET_S1,			// wire 328
  CH_A_OFFSET_S1,			// wire 329
  CH_A_OFFSET_S1,			// wire 330
  CH_A_OFFSET_S1,			// wire 331
  CH_A_OFFSET_S1,			// wire 332
  CH_A_OFFSET_S1,			// wire 333
  CH_A_OFFSET_S1,			// wire 334
  CH_A_OFFSET_S1,			// wire 335
  CH_A_OFFSET_S1,			// wire 336
  CH_A_OFFSET_S1,			// wire 337
  CH_A_OFFSET_S1,			// wire 338
  CH_A_OFFSET_S1,			// wire 339
  CH_A_OFFSET_S1,			// wire 340
  CH_A_OFFSET_S1,			// wire 341
  CH_A_OFFSET_S1,			// wire 342
  CH_A_OFFSET_S1,			// wire 343
  CH_A_OFFSET_S1,			// wire 344
  CH_A_OFFSET_S1,			// wire 345
  CH_A_OFFSET_S1,			// wire 346
  CH_A_OFFSET_S1,			// wire 347
  CH_A_OFFSET_S1,			// wire 348
  CH_A_OFFSET_S1,			// wire 349
  CH_A_OFFSET_S1,			// wire 350
  CH_A_OFFSET_S1,			// wire 351
  CH_A_OFFSET_S1			// wire 352
};

static short
offset_ax2[CH_A_MAX_X2] = {
  CH_A_OFFSET_X2,			// wire 1
  CH_A_OFFSET_X2,			// wire 2
  CH_A_OFFSET_X2,			// wire 3
  CH_A_OFFSET_X2,			// wire 4
  CH_A_OFFSET_X2,			// wire 5
  CH_A_OFFSET_X2,			// wire 6
  CH_A_OFFSET_X2,			// wire 7
  CH_A_OFFSET_X2,			// wire 8
  CH_A_OFFSET_X2,			// wire 9
  CH_A_OFFSET_X2,			// wire 10
  CH_A_OFFSET_X2,			// wire 11
  CH_A_OFFSET_X2,			// wire 12
  CH_A_OFFSET_X2,			// wire 13
  CH_A_OFFSET_X2,			// wire 14
  CH_A_OFFSET_X2,			// wire 15
  CH_A_OFFSET_X2,			// wire 16
  CH_A_OFFSET_X2,			// wire 17
  CH_A_OFFSET_X2,			// wire 18
  CH_A_OFFSET_X2,			// wire 19
  CH_A_OFFSET_X2,			// wire 20
  CH_A_OFFSET_X2,			// wire 21
  CH_A_OFFSET_X2,			// wire 22
  CH_A_OFFSET_X2,			// wire 23
  CH_A_OFFSET_X2,			// wire 24
  CH_A_OFFSET_X2,			// wire 25
  CH_A_OFFSET_X2,			// wire 26
  CH_A_OFFSET_X2,			// wire 27
  CH_A_OFFSET_X2,			// wire 28
  CH_A_OFFSET_X2,			// wire 29
  CH_A_OFFSET_X2,			// wire 30
  CH_A_OFFSET_X2,			// wire 31
  CH_A_OFFSET_X2,			// wire 32
  CH_A_OFFSET_X2,			// wire 33
  CH_A_OFFSET_X2,			// wire 34
  CH_A_OFFSET_X2,			// wire 35
  CH_A_OFFSET_X2,			// wire 36
  CH_A_OFFSET_X2,			// wire 37
  CH_A_OFFSET_X2,			// wire 38
  CH_A_OFFSET_X2,			// wire 39
  CH_A_OFFSET_X2,			// wire 40
  CH_A_OFFSET_X2,			// wire 41
  CH_A_OFFSET_X2,			// wire 42
  CH_A_OFFSET_X2,			// wire 43
  CH_A_OFFSET_X2,			// wire 44
  CH_A_OFFSET_X2,			// wire 45
  CH_A_OFFSET_X2,			// wire 46
  CH_A_OFFSET_X2,			// wire 47
  CH_A_OFFSET_X2,			// wire 48
  CH_A_OFFSET_X2,			// wire 49
  CH_A_OFFSET_X2,			// wire 50
  CH_A_OFFSET_X2,			// wire 51
  CH_A_OFFSET_X2,			// wire 52
  CH_A_OFFSET_X2,			// wire 53
  CH_A_OFFSET_X2,			// wire 54
  CH_A_OFFSET_X2,			// wire 55
  CH_A_OFFSET_X2,			// wire 56
  CH_A_OFFSET_X2,			// wire 57
  CH_A_OFFSET_X2,			// wire 58
  CH_A_OFFSET_X2,			// wire 59
  CH_A_OFFSET_X2,			// wire 60
  CH_A_OFFSET_X2,			// wire 61
  CH_A_OFFSET_X2,			// wire 62
  CH_A_OFFSET_X2,			// wire 63
  CH_A_OFFSET_X2,			// wire 64
  CH_A_OFFSET_X2,			// wire 65
  CH_A_OFFSET_X2,			// wire 66
  CH_A_OFFSET_X2,			// wire 67
  CH_A_OFFSET_X2,			// wire 68
  CH_A_OFFSET_X2,			// wire 69
  CH_A_OFFSET_X2,			// wire 70
  CH_A_OFFSET_X2,			// wire 71
  CH_A_OFFSET_X2,			// wire 72
  CH_A_OFFSET_X2,			// wire 73
  CH_A_OFFSET_X2,			// wire 74
  CH_A_OFFSET_X2,			// wire 75
  CH_A_OFFSET_X2,			// wire 76
  CH_A_OFFSET_X2,			// wire 77
  CH_A_OFFSET_X2,			// wire 78
  CH_A_OFFSET_X2,			// wire 79
  CH_A_OFFSET_X2,			// wire 80
  CH_A_OFFSET_X2,			// wire 81
  CH_A_OFFSET_X2,			// wire 82
  CH_A_OFFSET_X2,			// wire 83
  CH_A_OFFSET_X2,			// wire 84
  CH_A_OFFSET_X2,			// wire 85
  CH_A_OFFSET_X2,			// wire 86
  CH_A_OFFSET_X2,			// wire 87
  CH_A_OFFSET_X2,			// wire 88
  CH_A_OFFSET_X2,			// wire 89
  CH_A_OFFSET_X2,			// wire 90
  CH_A_OFFSET_X2,			// wire 91
  CH_A_OFFSET_X2,			// wire 92
  CH_A_OFFSET_X2,			// wire 93
  CH_A_OFFSET_X2,			// wire 94
  CH_A_OFFSET_X2,			// wire 95
  CH_A_OFFSET_X2,			// wire 96
  CH_A_OFFSET_X2,			// wire 97
  CH_A_OFFSET_X2,			// wire 98
  CH_A_OFFSET_X2,			// wire 99
  CH_A_OFFSET_X2,			// wire 100
  CH_A_OFFSET_X2,			// wire 101
  CH_A_OFFSET_X2,			// wire 102
  CH_A_OFFSET_X2,			// wire 103
  CH_A_OFFSET_X2,			// wire 104
  CH_A_OFFSET_X2,			// wire 105
  CH_A_OFFSET_X2,			// wire 106
  CH_A_OFFSET_X2,			// wire 107
  CH_A_OFFSET_X2,			// wire 108
  CH_A_OFFSET_X2,			// wire 109
  CH_A_OFFSET_X2,			// wire 110
  CH_A_OFFSET_X2,			// wire 111
  CH_A_OFFSET_X2,			// wire 112
  CH_A_OFFSET_X2,			// wire 113
  CH_A_OFFSET_X2,			// wire 114
  CH_A_OFFSET_X2,			// wire 115
  CH_A_OFFSET_X2,			// wire 116
  CH_A_OFFSET_X2,			// wire 117
  CH_A_OFFSET_X2,			// wire 118
  CH_A_OFFSET_X2,			// wire 119
  CH_A_OFFSET_X2,			// wire 120
  CH_A_OFFSET_X2,			// wire 121
  CH_A_OFFSET_X2,			// wire 122
  CH_A_OFFSET_X2,			// wire 123
  CH_A_OFFSET_X2,			// wire 124
  CH_A_OFFSET_X2,			// wire 125
  CH_A_OFFSET_X2,			// wire 126
  CH_A_OFFSET_X2,			// wire 127
  CH_A_OFFSET_X2,			// wire 128
  CH_A_OFFSET_X2,			// wire 129
  CH_A_OFFSET_X2,			// wire 130
  CH_A_OFFSET_X2,			// wire 131
  CH_A_OFFSET_X2,			// wire 132
  CH_A_OFFSET_X2,			// wire 133
  CH_A_OFFSET_X2,			// wire 134
  CH_A_OFFSET_X2,			// wire 135
  CH_A_OFFSET_X2,			// wire 136
  CH_A_OFFSET_X2,			// wire 137
  CH_A_OFFSET_X2,			// wire 138
  CH_A_OFFSET_X2,			// wire 139
  CH_A_OFFSET_X2,			// wire 140
  CH_A_OFFSET_X2,			// wire 141
  CH_A_OFFSET_X2,			// wire 142
  CH_A_OFFSET_X2,			// wire 143
  CH_A_OFFSET_X2,			// wire 144
  CH_A_OFFSET_X2,			// wire 145
  CH_A_OFFSET_X2,			// wire 146
  CH_A_OFFSET_X2,			// wire 147
  CH_A_OFFSET_X2,			// wire 148
  CH_A_OFFSET_X2,			// wire 149
  CH_A_OFFSET_X2,			// wire 150
  CH_A_OFFSET_X2,			// wire 151
  CH_A_OFFSET_X2,			// wire 152
  CH_A_OFFSET_X2,			// wire 153
  CH_A_OFFSET_X2,			// wire 154
  CH_A_OFFSET_X2,			// wire 155
  CH_A_OFFSET_X2,			// wire 156
  CH_A_OFFSET_X2,			// wire 157
  CH_A_OFFSET_X2,			// wire 158
  CH_A_OFFSET_X2,			// wire 159
  CH_A_OFFSET_X2,			// wire 160
  CH_A_OFFSET_X2,			// wire 161
  CH_A_OFFSET_X2,			// wire 162
  CH_A_OFFSET_X2,			// wire 163
  CH_A_OFFSET_X2,			// wire 164
  CH_A_OFFSET_X2,			// wire 165
  CH_A_OFFSET_X2,			// wire 166
  CH_A_OFFSET_X2,			// wire 167
  CH_A_OFFSET_X2,			// wire 168
  CH_A_OFFSET_X2,			// wire 169
  CH_A_OFFSET_X2,			// wire 170
  CH_A_OFFSET_X2,			// wire 171
  CH_A_OFFSET_X2,			// wire 172
  CH_A_OFFSET_X2,			// wire 173
  CH_A_OFFSET_X2,			// wire 174
  CH_A_OFFSET_X2,			// wire 175
  CH_A_OFFSET_X2,			// wire 176
  CH_A_OFFSET_X2,			// wire 177
  CH_A_OFFSET_X2,			// wire 178
  CH_A_OFFSET_X2,			// wire 179
  CH_A_OFFSET_X2,			// wire 180
  CH_A_OFFSET_X2,			// wire 181
  CH_A_OFFSET_X2,			// wire 182
  CH_A_OFFSET_X2,			// wire 183
  CH_A_OFFSET_X2,			// wire 184
  CH_A_OFFSET_X2,			// wire 185
  CH_A_OFFSET_X2,			// wire 186
  CH_A_OFFSET_X2,			// wire 187
  CH_A_OFFSET_X2,			// wire 188
  CH_A_OFFSET_X2,			// wire 189
  CH_A_OFFSET_X2,			// wire 190
  CH_A_OFFSET_X2,			// wire 191
  CH_A_OFFSET_X2,			// wire 192
  CH_A_OFFSET_X2,			// wire 193
  CH_A_OFFSET_X2,			// wire 194
  CH_A_OFFSET_X2,			// wire 195
  CH_A_OFFSET_X2,			// wire 196
  CH_A_OFFSET_X2,			// wire 197
  CH_A_OFFSET_X2,			// wire 198
  CH_A_OFFSET_X2,			// wire 199
  CH_A_OFFSET_X2,			// wire 200
  CH_A_OFFSET_X2,			// wire 201
  CH_A_OFFSET_X2,			// wire 202
  CH_A_OFFSET_X2,			// wire 203
  CH_A_OFFSET_X2,			// wire 204
  CH_A_OFFSET_X2,			// wire 205
  CH_A_OFFSET_X2,			// wire 206
  CH_A_OFFSET_X2,			// wire 207
  CH_A_OFFSET_X2,			// wire 208
  CH_A_OFFSET_X2,			// wire 209
  CH_A_OFFSET_X2,			// wire 210
  CH_A_OFFSET_X2,			// wire 211
  CH_A_OFFSET_X2,			// wire 212
  CH_A_OFFSET_X2,			// wire 213
  CH_A_OFFSET_X2,			// wire 214
  CH_A_OFFSET_X2,			// wire 215
  CH_A_OFFSET_X2,			// wire 216
  CH_A_OFFSET_X2,			// wire 217
  CH_A_OFFSET_X2,			// wire 218
  CH_A_OFFSET_X2,			// wire 219
  CH_A_OFFSET_X2,			// wire 220
  CH_A_OFFSET_X2,			// wire 221
  CH_A_OFFSET_X2,			// wire 222
  CH_A_OFFSET_X2,			// wire 223
  CH_A_OFFSET_X2,			// wire 224
  CH_A_OFFSET_X2,			// wire 225
  CH_A_OFFSET_X2,			// wire 226
  CH_A_OFFSET_X2,			// wire 227
  CH_A_OFFSET_X2,			// wire 228
  CH_A_OFFSET_X2,			// wire 229
  CH_A_OFFSET_X2,			// wire 230
  CH_A_OFFSET_X2,			// wire 231
  CH_A_OFFSET_X2,			// wire 232
  CH_A_OFFSET_X2,			// wire 233
  CH_A_OFFSET_X2,			// wire 234
  CH_A_OFFSET_X2,			// wire 235
  CH_A_OFFSET_X2,			// wire 236
  CH_A_OFFSET_X2,			// wire 237
  CH_A_OFFSET_X2,			// wire 238
  CH_A_OFFSET_X2,			// wire 239
  CH_A_OFFSET_X2,			// wire 240
  CH_A_OFFSET_X2,			// wire 241
  CH_A_OFFSET_X2,			// wire 242
  CH_A_OFFSET_X2,			// wire 243
  CH_A_OFFSET_X2,			// wire 244
  CH_A_OFFSET_X2,			// wire 245
  CH_A_OFFSET_X2,			// wire 246
  CH_A_OFFSET_X2,			// wire 247
  CH_A_OFFSET_X2,			// wire 248
  CH_A_OFFSET_X2,			// wire 249
  CH_A_OFFSET_X2,			// wire 250
  CH_A_OFFSET_X2,			// wire 251
  CH_A_OFFSET_X2,			// wire 252
  CH_A_OFFSET_X2,			// wire 253
  CH_A_OFFSET_X2,			// wire 254
  CH_A_OFFSET_X2,			// wire 255
  CH_A_OFFSET_X2,			// wire 256
  CH_A_OFFSET_X2,			// wire 257
  CH_A_OFFSET_X2,			// wire 258
  CH_A_OFFSET_X2,			// wire 259
  CH_A_OFFSET_X2,			// wire 260
  CH_A_OFFSET_X2,			// wire 261
  CH_A_OFFSET_X2,			// wire 262
  CH_A_OFFSET_X2,			// wire 263
  CH_A_OFFSET_X2,			// wire 264
  CH_A_OFFSET_X2,			// wire 265
  CH_A_OFFSET_X2,			// wire 266
  CH_A_OFFSET_X2,			// wire 267
  CH_A_OFFSET_X2,			// wire 268
  CH_A_OFFSET_X2,			// wire 269
  CH_A_OFFSET_X2,			// wire 270
  CH_A_OFFSET_X2,			// wire 271
  CH_A_OFFSET_X2,			// wire 272
  CH_A_OFFSET_X2,			// wire 273
  CH_A_OFFSET_X2,			// wire 274
  CH_A_OFFSET_X2,			// wire 275
  CH_A_OFFSET_X2,			// wire 276
  CH_A_OFFSET_X2,			// wire 277
  CH_A_OFFSET_X2,			// wire 278
  CH_A_OFFSET_X2,			// wire 279
  CH_A_OFFSET_X2,			// wire 280
  CH_A_OFFSET_X2,			// wire 281
  CH_A_OFFSET_X2,			// wire 282
  CH_A_OFFSET_X2,			// wire 283
  CH_A_OFFSET_X2,			// wire 284
  CH_A_OFFSET_X2,			// wire 285
  CH_A_OFFSET_X2,			// wire 286
  CH_A_OFFSET_X2,			// wire 287
  CH_A_OFFSET_X2,			// wire 288
  CH_A_OFFSET_X2,			// wire 289
  CH_A_OFFSET_X2,			// wire 290
  CH_A_OFFSET_X2,			// wire 291
  CH_A_OFFSET_X2,			// wire 292
  CH_A_OFFSET_X2,			// wire 293
  CH_A_OFFSET_X2,			// wire 294
  CH_A_OFFSET_X2,			// wire 295
  CH_A_OFFSET_X2,			// wire 296
  CH_A_OFFSET_X2,			// wire 297
  CH_A_OFFSET_X2,			// wire 298
  CH_A_OFFSET_X2,			// wire 299
  CH_A_OFFSET_X2,			// wire 300
  CH_A_OFFSET_X2,			// wire 301
  CH_A_OFFSET_X2,			// wire 302
  CH_A_OFFSET_X2,			// wire 303
  CH_A_OFFSET_X2,			// wire 304
  CH_A_OFFSET_X2,			// wire 305
  CH_A_OFFSET_X2,			// wire 306
  CH_A_OFFSET_X2,			// wire 307
  CH_A_OFFSET_X2,			// wire 308
  CH_A_OFFSET_X2,			// wire 309
  CH_A_OFFSET_X2,			// wire 310
  CH_A_OFFSET_X2,			// wire 311
  CH_A_OFFSET_X2,			// wire 312
  CH_A_OFFSET_X2,			// wire 313
  CH_A_OFFSET_X2,			// wire 314
  CH_A_OFFSET_X2,			// wire 315
  CH_A_OFFSET_X2,			// wire 316
  CH_A_OFFSET_X2,			// wire 317
  CH_A_OFFSET_X2,			// wire 318
  CH_A_OFFSET_X2,			// wire 319
  CH_A_OFFSET_X2,			// wire 320
  CH_A_OFFSET_X2,			// wire 321
  CH_A_OFFSET_X2,			// wire 322
  CH_A_OFFSET_X2,			// wire 323
  CH_A_OFFSET_X2,			// wire 324
  CH_A_OFFSET_X2,			// wire 325
  CH_A_OFFSET_X2,			// wire 326
  CH_A_OFFSET_X2,			// wire 327
  CH_A_OFFSET_X2,			// wire 328
  CH_A_OFFSET_X2,			// wire 329
  CH_A_OFFSET_X2,			// wire 330
  CH_A_OFFSET_X2,			// wire 331
  CH_A_OFFSET_X2,			// wire 332
  CH_A_OFFSET_X2,			// wire 333
  CH_A_OFFSET_X2,			// wire 334
  CH_A_OFFSET_X2,			// wire 335
  CH_A_OFFSET_X2,			// wire 336
  CH_A_OFFSET_X2,			// wire 337
  CH_A_OFFSET_X2,			// wire 338
  CH_A_OFFSET_X2,			// wire 339
  CH_A_OFFSET_X2,			// wire 340
  CH_A_OFFSET_X2,			// wire 341
  CH_A_OFFSET_X2,			// wire 342
  CH_A_OFFSET_X2,			// wire 343
  CH_A_OFFSET_X2,			// wire 344
  CH_A_OFFSET_X2,			// wire 345
  CH_A_OFFSET_X2,			// wire 346
  CH_A_OFFSET_X2,			// wire 347
  CH_A_OFFSET_X2,			// wire 348
  CH_A_OFFSET_X2,			// wire 349
  CH_A_OFFSET_X2,			// wire 350
  CH_A_OFFSET_X2,			// wire 351
  CH_A_OFFSET_X2,			// wire 352
  CH_A_OFFSET_X2,			// wire 353
  CH_A_OFFSET_X2,			// wire 354
  CH_A_OFFSET_X2,			// wire 355
  CH_A_OFFSET_X2,			// wire 356
  CH_A_OFFSET_X2,			// wire 357
  CH_A_OFFSET_X2,			// wire 358
  CH_A_OFFSET_X2,			// wire 359
  CH_A_OFFSET_X2,			// wire 360
  CH_A_OFFSET_X2,			// wire 361
  CH_A_OFFSET_X2,			// wire 362
  CH_A_OFFSET_X2,			// wire 363
  CH_A_OFFSET_X2,			// wire 364
  CH_A_OFFSET_X2,			// wire 365
  CH_A_OFFSET_X2,			// wire 366
  CH_A_OFFSET_X2,			// wire 367
  CH_A_OFFSET_X2,			// wire 368
  CH_A_OFFSET_X2,			// wire 369
  CH_A_OFFSET_X2,			// wire 370
  CH_A_OFFSET_X2,			// wire 371
  CH_A_OFFSET_X2,			// wire 372
  CH_A_OFFSET_X2,			// wire 373
  CH_A_OFFSET_X2,			// wire 374
  CH_A_OFFSET_X2,			// wire 375
  CH_A_OFFSET_X2,			// wire 376
  CH_A_OFFSET_X2,			// wire 377
  CH_A_OFFSET_X2,			// wire 378
  CH_A_OFFSET_X2,			// wire 379
  CH_A_OFFSET_X2,			// wire 380
  CH_A_OFFSET_X2,			// wire 381
  CH_A_OFFSET_X2,			// wire 382
  CH_A_OFFSET_X2,			// wire 383
  CH_A_OFFSET_X2,			// wire 384
  CH_A_OFFSET_X2,			// wire 385
  CH_A_OFFSET_X2,			// wire 386
  CH_A_OFFSET_X2,			// wire 387
  CH_A_OFFSET_X2,			// wire 388
  CH_A_OFFSET_X2,			// wire 389
  CH_A_OFFSET_X2,			// wire 390
  CH_A_OFFSET_X2,			// wire 391
  CH_A_OFFSET_X2,			// wire 392
  CH_A_OFFSET_X2,			// wire 393
  CH_A_OFFSET_X2,			// wire 394
  CH_A_OFFSET_X2,			// wire 395
  CH_A_OFFSET_X2,			// wire 396
  CH_A_OFFSET_X2,			// wire 397
  CH_A_OFFSET_X2,			// wire 398
  CH_A_OFFSET_X2,			// wire 399
  CH_A_OFFSET_X2,			// wire 400
  CH_A_OFFSET_X2,			// wire 401
  CH_A_OFFSET_X2,			// wire 402
  CH_A_OFFSET_X2,			// wire 403
  CH_A_OFFSET_X2,			// wire 404
  CH_A_OFFSET_X2,			// wire 405
  CH_A_OFFSET_X2,			// wire 406
  CH_A_OFFSET_X2,			// wire 407
  CH_A_OFFSET_X2,			// wire 408
  CH_A_OFFSET_X2,			// wire 409
  CH_A_OFFSET_X2,			// wire 410
  CH_A_OFFSET_X2,			// wire 411
  CH_A_OFFSET_X2,			// wire 412
  CH_A_OFFSET_X2,			// wire 413
  CH_A_OFFSET_X2,			// wire 414
  CH_A_OFFSET_X2,			// wire 415
  CH_A_OFFSET_X2			// wire 416
};

static short
offset_as2[CH_A_MAX_S2] = {
  CH_A_OFFSET_S2,			// wire 1
  CH_A_OFFSET_S2,			// wire 2
  CH_A_OFFSET_S2,			// wire 3
  CH_A_OFFSET_S2,			// wire 4
  CH_A_OFFSET_S2,			// wire 5
  CH_A_OFFSET_S2,			// wire 6
  CH_A_OFFSET_S2,			// wire 7
  CH_A_OFFSET_S2,			// wire 8
  CH_A_OFFSET_S2,			// wire 9
  CH_A_OFFSET_S2,			// wire 10
  CH_A_OFFSET_S2,			// wire 11
  CH_A_OFFSET_S2,			// wire 12
  CH_A_OFFSET_S2,			// wire 13
  CH_A_OFFSET_S2,			// wire 14
  CH_A_OFFSET_S2,			// wire 15
  CH_A_OFFSET_S2,			// wire 16
  CH_A_OFFSET_S2,			// wire 17
  CH_A_OFFSET_S2,			// wire 18
  CH_A_OFFSET_S2,			// wire 19
  CH_A_OFFSET_S2,			// wire 20
  CH_A_OFFSET_S2,			// wire 21
  CH_A_OFFSET_S2,			// wire 22
  CH_A_OFFSET_S2,			// wire 23
  CH_A_OFFSET_S2,			// wire 24
  CH_A_OFFSET_S2,			// wire 25
  CH_A_OFFSET_S2,			// wire 26
  CH_A_OFFSET_S2,			// wire 27
  CH_A_OFFSET_S2,			// wire 28
  CH_A_OFFSET_S2,			// wire 29
  CH_A_OFFSET_S2,			// wire 30
  CH_A_OFFSET_S2,			// wire 31
  CH_A_OFFSET_S2,			// wire 32
  CH_A_OFFSET_S2,			// wire 33
  CH_A_OFFSET_S2,			// wire 34
  CH_A_OFFSET_S2,			// wire 35
  CH_A_OFFSET_S2,			// wire 36
  CH_A_OFFSET_S2,			// wire 37
  CH_A_OFFSET_S2,			// wire 38
  CH_A_OFFSET_S2,			// wire 39
  CH_A_OFFSET_S2,			// wire 40
  CH_A_OFFSET_S2,			// wire 41
  CH_A_OFFSET_S2,			// wire 42
  CH_A_OFFSET_S2,			// wire 43
  CH_A_OFFSET_S2,			// wire 44
  CH_A_OFFSET_S2,			// wire 45
  CH_A_OFFSET_S2,			// wire 46
  CH_A_OFFSET_S2,			// wire 47
  CH_A_OFFSET_S2,			// wire 48
  CH_A_OFFSET_S2,			// wire 49
  CH_A_OFFSET_S2,			// wire 50
  CH_A_OFFSET_S2,			// wire 51
  CH_A_OFFSET_S2,			// wire 52
  CH_A_OFFSET_S2,			// wire 53
  CH_A_OFFSET_S2,			// wire 54
  CH_A_OFFSET_S2,			// wire 55
  CH_A_OFFSET_S2,			// wire 56
  CH_A_OFFSET_S2,			// wire 57
  CH_A_OFFSET_S2,			// wire 58
  CH_A_OFFSET_S2,			// wire 59
  CH_A_OFFSET_S2,			// wire 60
  CH_A_OFFSET_S2,			// wire 61
  CH_A_OFFSET_S2,			// wire 62
  CH_A_OFFSET_S2,			// wire 63
  CH_A_OFFSET_S2,			// wire 64
  CH_A_OFFSET_S2,			// wire 65
  CH_A_OFFSET_S2,			// wire 66
  CH_A_OFFSET_S2,			// wire 67
  CH_A_OFFSET_S2,			// wire 68
  CH_A_OFFSET_S2,			// wire 69
  CH_A_OFFSET_S2,			// wire 70
  CH_A_OFFSET_S2,			// wire 71
  CH_A_OFFSET_S2,			// wire 72
  CH_A_OFFSET_S2,			// wire 73
  CH_A_OFFSET_S2,			// wire 74
  CH_A_OFFSET_S2,			// wire 75
  CH_A_OFFSET_S2,			// wire 76
  CH_A_OFFSET_S2,			// wire 77
  CH_A_OFFSET_S2,			// wire 78
  CH_A_OFFSET_S2,			// wire 79
  CH_A_OFFSET_S2,			// wire 80
  CH_A_OFFSET_S2,			// wire 81
  CH_A_OFFSET_S2,			// wire 82
  CH_A_OFFSET_S2,			// wire 83
  CH_A_OFFSET_S2,			// wire 84
  CH_A_OFFSET_S2,			// wire 85
  CH_A_OFFSET_S2,			// wire 86
  CH_A_OFFSET_S2,			// wire 87
  CH_A_OFFSET_S2,			// wire 88
  CH_A_OFFSET_S2,			// wire 89
  CH_A_OFFSET_S2,			// wire 90
  CH_A_OFFSET_S2,			// wire 91
  CH_A_OFFSET_S2,			// wire 92
  CH_A_OFFSET_S2,			// wire 93
  CH_A_OFFSET_S2,			// wire 94
  CH_A_OFFSET_S2,			// wire 95
  CH_A_OFFSET_S2,			// wire 96
  CH_A_OFFSET_S2,			// wire 97
  CH_A_OFFSET_S2,			// wire 98
  CH_A_OFFSET_S2,			// wire 99
  CH_A_OFFSET_S2,			// wire 100
  CH_A_OFFSET_S2,			// wire 101
  CH_A_OFFSET_S2,			// wire 102
  CH_A_OFFSET_S2,			// wire 103
  CH_A_OFFSET_S2,			// wire 104
  CH_A_OFFSET_S2,			// wire 105
  CH_A_OFFSET_S2,			// wire 106
  CH_A_OFFSET_S2,			// wire 107
  CH_A_OFFSET_S2,			// wire 108
  CH_A_OFFSET_S2,			// wire 109
  CH_A_OFFSET_S2,			// wire 110
  CH_A_OFFSET_S2,			// wire 111
  CH_A_OFFSET_S2,			// wire 112
  CH_A_OFFSET_S2,			// wire 113
  CH_A_OFFSET_S2,			// wire 114
  CH_A_OFFSET_S2,			// wire 115
  CH_A_OFFSET_S2,			// wire 116
  CH_A_OFFSET_S2,			// wire 117
  CH_A_OFFSET_S2,			// wire 118
  CH_A_OFFSET_S2,			// wire 119
  CH_A_OFFSET_S2,			// wire 120
  CH_A_OFFSET_S2,			// wire 121
  CH_A_OFFSET_S2,			// wire 122
  CH_A_OFFSET_S2,			// wire 123
  CH_A_OFFSET_S2,			// wire 124
  CH_A_OFFSET_S2,			// wire 125
  CH_A_OFFSET_S2,			// wire 126
  CH_A_OFFSET_S2,			// wire 127
  CH_A_OFFSET_S2,			// wire 128
  CH_A_OFFSET_S2,			// wire 129
  CH_A_OFFSET_S2,			// wire 130
  CH_A_OFFSET_S2,			// wire 131
  CH_A_OFFSET_S2,			// wire 132
  CH_A_OFFSET_S2,			// wire 133
  CH_A_OFFSET_S2,			// wire 134
  CH_A_OFFSET_S2,			// wire 135
  CH_A_OFFSET_S2,			// wire 136
  CH_A_OFFSET_S2,			// wire 137
  CH_A_OFFSET_S2,			// wire 138
  CH_A_OFFSET_S2,			// wire 139
  CH_A_OFFSET_S2,			// wire 140
  CH_A_OFFSET_S2,			// wire 141
  CH_A_OFFSET_S2,			// wire 142
  CH_A_OFFSET_S2,			// wire 143
  CH_A_OFFSET_S2,			// wire 144
  CH_A_OFFSET_S2,			// wire 145
  CH_A_OFFSET_S2,			// wire 146
  CH_A_OFFSET_S2,			// wire 147
  CH_A_OFFSET_S2,			// wire 148
  CH_A_OFFSET_S2,			// wire 149
  CH_A_OFFSET_S2,			// wire 150
  CH_A_OFFSET_S2,			// wire 151
  CH_A_OFFSET_S2,			// wire 152
  CH_A_OFFSET_S2,			// wire 153
  CH_A_OFFSET_S2,			// wire 154
  CH_A_OFFSET_S2,			// wire 155
  CH_A_OFFSET_S2,			// wire 156
  CH_A_OFFSET_S2,			// wire 157
  CH_A_OFFSET_S2,			// wire 158
  CH_A_OFFSET_S2,			// wire 159
  CH_A_OFFSET_S2,			// wire 160
  CH_A_OFFSET_S2,			// wire 161
  CH_A_OFFSET_S2,			// wire 162
  CH_A_OFFSET_S2,			// wire 163
  CH_A_OFFSET_S2,			// wire 164
  CH_A_OFFSET_S2,			// wire 165
  CH_A_OFFSET_S2,			// wire 166
  CH_A_OFFSET_S2,			// wire 167
  CH_A_OFFSET_S2,			// wire 168
  CH_A_OFFSET_S2,			// wire 169
  CH_A_OFFSET_S2,			// wire 170
  CH_A_OFFSET_S2,			// wire 171
  CH_A_OFFSET_S2,			// wire 172
  CH_A_OFFSET_S2,			// wire 173
  CH_A_OFFSET_S2,			// wire 174
  CH_A_OFFSET_S2,			// wire 175
  CH_A_OFFSET_S2,			// wire 176
  CH_A_OFFSET_S2,			// wire 177
  CH_A_OFFSET_S2,			// wire 178
  CH_A_OFFSET_S2,			// wire 179
  CH_A_OFFSET_S2,			// wire 180
  CH_A_OFFSET_S2,			// wire 181
  CH_A_OFFSET_S2,			// wire 182
  CH_A_OFFSET_S2,			// wire 183
  CH_A_OFFSET_S2,			// wire 184
  CH_A_OFFSET_S2,			// wire 185
  CH_A_OFFSET_S2,			// wire 186
  CH_A_OFFSET_S2,			// wire 187
  CH_A_OFFSET_S2,			// wire 188
  CH_A_OFFSET_S2,			// wire 189
  CH_A_OFFSET_S2,			// wire 190
  CH_A_OFFSET_S2,			// wire 191
  CH_A_OFFSET_S2,			// wire 192
  CH_A_OFFSET_S2,			// wire 193
  CH_A_OFFSET_S2,			// wire 194
  CH_A_OFFSET_S2,			// wire 195
  CH_A_OFFSET_S2,			// wire 196
  CH_A_OFFSET_S2,			// wire 197
  CH_A_OFFSET_S2,			// wire 198
  CH_A_OFFSET_S2,			// wire 199
  CH_A_OFFSET_S2,			// wire 200
  CH_A_OFFSET_S2,			// wire 201
  CH_A_OFFSET_S2,			// wire 202
  CH_A_OFFSET_S2,			// wire 203
  CH_A_OFFSET_S2,			// wire 204
  CH_A_OFFSET_S2,			// wire 205
  CH_A_OFFSET_S2,			// wire 206
  CH_A_OFFSET_S2,			// wire 207
  CH_A_OFFSET_S2,			// wire 208
  CH_A_OFFSET_S2,			// wire 209
  CH_A_OFFSET_S2,			// wire 210
  CH_A_OFFSET_S2,			// wire 211
  CH_A_OFFSET_S2,			// wire 212
  CH_A_OFFSET_S2,			// wire 213
  CH_A_OFFSET_S2,			// wire 214
  CH_A_OFFSET_S2,			// wire 215
  CH_A_OFFSET_S2,			// wire 216
  CH_A_OFFSET_S2,			// wire 217
  CH_A_OFFSET_S2,			// wire 218
  CH_A_OFFSET_S2,			// wire 219
  CH_A_OFFSET_S2,			// wire 220
  CH_A_OFFSET_S2,			// wire 221
  CH_A_OFFSET_S2,			// wire 222
  CH_A_OFFSET_S2,			// wire 223
  CH_A_OFFSET_S2,			// wire 224
  CH_A_OFFSET_S2,			// wire 225
  CH_A_OFFSET_S2,			// wire 226
  CH_A_OFFSET_S2,			// wire 227
  CH_A_OFFSET_S2,			// wire 228
  CH_A_OFFSET_S2,			// wire 229
  CH_A_OFFSET_S2,			// wire 230
  CH_A_OFFSET_S2,			// wire 231
  CH_A_OFFSET_S2,			// wire 232
  CH_A_OFFSET_S2,			// wire 233
  CH_A_OFFSET_S2,			// wire 234
  CH_A_OFFSET_S2,			// wire 235
  CH_A_OFFSET_S2,			// wire 236
  CH_A_OFFSET_S2,			// wire 237
  CH_A_OFFSET_S2,			// wire 238
  CH_A_OFFSET_S2,			// wire 239
  CH_A_OFFSET_S2,			// wire 240
  CH_A_OFFSET_S2,			// wire 241
  CH_A_OFFSET_S2,			// wire 242
  CH_A_OFFSET_S2,			// wire 243
  CH_A_OFFSET_S2,			// wire 244
  CH_A_OFFSET_S2,			// wire 245
  CH_A_OFFSET_S2,			// wire 246
  CH_A_OFFSET_S2,			// wire 247
  CH_A_OFFSET_S2,			// wire 248
  CH_A_OFFSET_S2,			// wire 249
  CH_A_OFFSET_S2,			// wire 250
  CH_A_OFFSET_S2,			// wire 251
  CH_A_OFFSET_S2,			// wire 252
  CH_A_OFFSET_S2,			// wire 253
  CH_A_OFFSET_S2,			// wire 254
  CH_A_OFFSET_S2,			// wire 255
  CH_A_OFFSET_S2,			// wire 256
  CH_A_OFFSET_S2,			// wire 257
  CH_A_OFFSET_S2,			// wire 258
  CH_A_OFFSET_S2,			// wire 259
  CH_A_OFFSET_S2,			// wire 260
  CH_A_OFFSET_S2,			// wire 261
  CH_A_OFFSET_S2,			// wire 262
  CH_A_OFFSET_S2,			// wire 263
  CH_A_OFFSET_S2,			// wire 264
  CH_A_OFFSET_S2,			// wire 265
  CH_A_OFFSET_S2,			// wire 266
  CH_A_OFFSET_S2,			// wire 267
  CH_A_OFFSET_S2,			// wire 268
  CH_A_OFFSET_S2,			// wire 269
  CH_A_OFFSET_S2,			// wire 270
  CH_A_OFFSET_S2,			// wire 271
  CH_A_OFFSET_S2,			// wire 272
  CH_A_OFFSET_S2,			// wire 273
  CH_A_OFFSET_S2,			// wire 274
  CH_A_OFFSET_S2,			// wire 275
  CH_A_OFFSET_S2,			// wire 276
  CH_A_OFFSET_S2,			// wire 277
  CH_A_OFFSET_S2,			// wire 278
  CH_A_OFFSET_S2,			// wire 279
  CH_A_OFFSET_S2,			// wire 280
  CH_A_OFFSET_S2,			// wire 281
  CH_A_OFFSET_S2,			// wire 282
  CH_A_OFFSET_S2,			// wire 283
  CH_A_OFFSET_S2,			// wire 284
  CH_A_OFFSET_S2,			// wire 285
  CH_A_OFFSET_S2,			// wire 286
  CH_A_OFFSET_S2,			// wire 287
  CH_A_OFFSET_S2,			// wire 288
  CH_A_OFFSET_S2,			// wire 289
  CH_A_OFFSET_S2,			// wire 290
  CH_A_OFFSET_S2,			// wire 291
  CH_A_OFFSET_S2,			// wire 292
  CH_A_OFFSET_S2,			// wire 293
  CH_A_OFFSET_S2,			// wire 294
  CH_A_OFFSET_S2,			// wire 295
  CH_A_OFFSET_S2,			// wire 296
  CH_A_OFFSET_S2,			// wire 297
  CH_A_OFFSET_S2,			// wire 298
  CH_A_OFFSET_S2,			// wire 299
  CH_A_OFFSET_S2,			// wire 300
  CH_A_OFFSET_S2,			// wire 301
  CH_A_OFFSET_S2,			// wire 302
  CH_A_OFFSET_S2,			// wire 303
  CH_A_OFFSET_S2,			// wire 304
  CH_A_OFFSET_S2,			// wire 305
  CH_A_OFFSET_S2,			// wire 306
  CH_A_OFFSET_S2,			// wire 307
  CH_A_OFFSET_S2,			// wire 308
  CH_A_OFFSET_S2,			// wire 309
  CH_A_OFFSET_S2,			// wire 310
  CH_A_OFFSET_S2,			// wire 311
  CH_A_OFFSET_S2,			// wire 312
  CH_A_OFFSET_S2,			// wire 313
  CH_A_OFFSET_S2,			// wire 314
  CH_A_OFFSET_S2,			// wire 315
  CH_A_OFFSET_S2,			// wire 316
  CH_A_OFFSET_S2,			// wire 317
  CH_A_OFFSET_S2,			// wire 318
  CH_A_OFFSET_S2,			// wire 319
  CH_A_OFFSET_S2,			// wire 320
  CH_A_OFFSET_S2-12,			// wire 321
  CH_A_OFFSET_S2-12,			// wire 322
  CH_A_OFFSET_S2-12,			// wire 323
  CH_A_OFFSET_S2-12,			// wire 324
  CH_A_OFFSET_S2-12,			// wire 325
  CH_A_OFFSET_S2-12,			// wire 326
  CH_A_OFFSET_S2-12,			// wire 327
  CH_A_OFFSET_S2-12,			// wire 328
  CH_A_OFFSET_S2-12,			// wire 329
  CH_A_OFFSET_S2-12,			// wire 330
  CH_A_OFFSET_S2-12,			// wire 331
  CH_A_OFFSET_S2-12,			// wire 332
  CH_A_OFFSET_S2-12,			// wire 333
  CH_A_OFFSET_S2-12,			// wire 334
  CH_A_OFFSET_S2-12,			// wire 335
  CH_A_OFFSET_S2-12,			// wire 336
  CH_A_OFFSET_S2-12,			// wire 337
  CH_A_OFFSET_S2-12,			// wire 338
  CH_A_OFFSET_S2-12,			// wire 339
  CH_A_OFFSET_S2-12,			// wire 340
  CH_A_OFFSET_S2-12,			// wire 341
  CH_A_OFFSET_S2-12,			// wire 342
  CH_A_OFFSET_S2-12,			// wire 343
  CH_A_OFFSET_S2-12,			// wire 344
  CH_A_OFFSET_S2-12,			// wire 345
  CH_A_OFFSET_S2-12,			// wire 346
  CH_A_OFFSET_S2-12,			// wire 347
  CH_A_OFFSET_S2-12,			// wire 348
  CH_A_OFFSET_S2-12,			// wire 349
  CH_A_OFFSET_S2-12,			// wire 350
  CH_A_OFFSET_S2-12,			// wire 351
  CH_A_OFFSET_S2-12			// wire 352
};

static float
position_ax1[CH_A_MAX_X1] = {
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
   945.57,		// x1 wire 96
   940.57,		// x1 wire 97
   935.57,		// x1 wire 98
   930.57,		// x1 wire 99
   925.57,		// x1 wire 100
   920.57,		// x1 wire 101
   915.57,		// x1 wire 102
   910.57,		// x1 wire 103
   905.57,		// x1 wire 104
   900.57,		// x1 wire 105
   895.57,		// x1 wire 106
   890.57,		// x1 wire 107
   885.57,		// x1 wire 108
   880.57,		// x1 wire 109
   875.57,		// x1 wire 110
   870.57,		// x1 wire 111
   865.57,		// x1 wire 112
   860.57,		// x1 wire 113
   855.57,		// x1 wire 114
   850.57,		// x1 wire 115
   845.57,		// x1 wire 116
   840.57,		// x1 wire 117
   835.57,		// x1 wire 118
   830.57,		// x1 wire 119
   825.57,		// x1 wire 120
   820.57,		// x1 wire 121
   815.57,		// x1 wire 122
   810.57,		// x1 wire 123
   805.57,		// x1 wire 124
   800.57,		// x1 wire 125
   795.57,		// x1 wire 126
   790.57,		// x1 wire 127
   785.57,		// x1 wire 128
   780.57,		// x1 wire 129
   775.57,		// x1 wire 130
   770.57,		// x1 wire 131
   765.57,		// x1 wire 132
   760.57,		// x1 wire 133
   755.57,		// x1 wire 134
   750.57,		// x1 wire 135
   745.57,		// x1 wire 136
   740.57,		// x1 wire 137
   735.57,		// x1 wire 138
   730.57,		// x1 wire 139
   725.57,		// x1 wire 140
   720.57,		// x1 wire 141
   715.57,		// x1 wire 142
   710.57,		// x1 wire 143
   705.57,		// x1 wire 144
   700.57,		// x1 wire 145
   695.57,		// x1 wire 146
   690.57,		// x1 wire 147
   685.57,		// x1 wire 148
   680.57,		// x1 wire 149
   675.57,		// x1 wire 150
   670.57,		// x1 wire 151
   665.57,		// x1 wire 152
   660.57,		// x1 wire 153
   655.57,		// x1 wire 154
   650.57,		// x1 wire 155
   645.57,		// x1 wire 156
   640.57,		// x1 wire 157
   635.57,		// x1 wire 158
   630.57,		// x1 wire 159
   625.57,		// x1 wire 160
   620.57,		// x1 wire 161
   615.57,		// x1 wire 162
   610.57,		// x1 wire 163
   605.57,		// x1 wire 164
   600.57,		// x1 wire 165
   595.57,		// x1 wire 166
   590.57,		// x1 wire 167
   585.57,		// x1 wire 168
   580.57,		// x1 wire 169
   575.57,		// x1 wire 170
   570.57,		// x1 wire 171
   565.57,		// x1 wire 172
   560.57,		// x1 wire 173
   555.57,		// x1 wire 174
   550.57,		// x1 wire 175
   545.57,		// x1 wire 176
   540.57,		// x1 wire 177
   535.57,		// x1 wire 178
   530.57,		// x1 wire 179
   525.57,		// x1 wire 180
   520.57,		// x1 wire 181
   515.57,		// x1 wire 182
   510.57,		// x1 wire 183
   505.57,		// x1 wire 184
   500.57,		// x1 wire 185
   495.57,		// x1 wire 186
   490.57,		// x1 wire 187
   485.57,		// x1 wire 188
   480.57,		// x1 wire 189
   475.57,		// x1 wire 190
   470.57,		// x1 wire 191
   465.57,		// x1 wire 192
   460.57,		// x1 wire 193
   455.57,		// x1 wire 194
   450.57,		// x1 wire 195
   445.57,		// x1 wire 196
   440.57,		// x1 wire 197
   435.57,		// x1 wire 198
   430.57,		// x1 wire 199
   425.57,		// x1 wire 200
   420.57,		// x1 wire 201
   415.57,		// x1 wire 202
   410.57,		// x1 wire 203
   405.57,		// x1 wire 204
   400.57,		// x1 wire 205
   395.57,		// x1 wire 206
   390.57,		// x1 wire 207
   385.57,		// x1 wire 208
   380.57,		// x1 wire 209
   375.57,		// x1 wire 210
   370.57,		// x1 wire 211
   365.57,		// x1 wire 212
   360.57,		// x1 wire 213
   355.57,		// x1 wire 214
   350.57,		// x1 wire 215
   345.57,		// x1 wire 216
   340.57,		// x1 wire 217
   335.57,		// x1 wire 218
   330.57,		// x1 wire 219
   325.57,		// x1 wire 220
   320.57,		// x1 wire 221
   315.57,		// x1 wire 222
   310.57,		// x1 wire 223
   305.57,		// x1 wire 224
   300.57,		// x1 wire 225
   295.57,		// x1 wire 226
   290.57,		// x1 wire 227
   285.57,		// x1 wire 228
   280.57,		// x1 wire 229
   275.57,		// x1 wire 230
   270.57,		// x1 wire 231
   265.57,		// x1 wire 232
   260.57,		// x1 wire 233
   255.57,		// x1 wire 234
   250.57,		// x1 wire 235
   245.57,		// x1 wire 236
   240.57,		// x1 wire 237
   235.57,		// x1 wire 238
   230.57,		// x1 wire 239
   225.57,		// x1 wire 240
   220.57,		// x1 wire 241
   215.57,		// x1 wire 242
   210.57,		// x1 wire 243
   205.57,		// x1 wire 244
   200.57,		// x1 wire 245
   195.57,		// x1 wire 246
   190.57,		// x1 wire 247
   185.57,		// x1 wire 248
   180.57,		// x1 wire 249
   175.57,		// x1 wire 250
   170.57,		// x1 wire 251
   165.57,		// x1 wire 252
   160.57,		// x1 wire 253
   155.57,		// x1 wire 254
   150.57,		// x1 wire 255
   145.57,		// x1 wire 256
   140.57,		// x1 wire 257
   135.57,		// x1 wire 258
   130.57,		// x1 wire 259
   125.57,		// x1 wire 260
   120.57,		// x1 wire 261
   115.57,		// x1 wire 262
   110.57,		// x1 wire 263
   105.57,		// x1 wire 264
   100.57,		// x1 wire 265
    95.57,		// x1 wire 266
    90.57,		// x1 wire 267
    85.57,		// x1 wire 268
    80.57,		// x1 wire 269
    75.57,		// x1 wire 270
    70.57,		// x1 wire 271
    65.57,		// x1 wire 272
    60.57,		// x1 wire 273
    55.57,		// x1 wire 274
    50.57,		// x1 wire 275
    45.57,		// x1 wire 276
    40.57,		// x1 wire 277
    35.57,		// x1 wire 278
    30.57,		// x1 wire 279
    25.57,		// x1 wire 280
    20.57,		// x1 wire 281
    15.57,		// x1 wire 282
    10.57,		// x1 wire 283
     5.57,		// x1 wire 284
     0.57,		// x1 wire 285
    -4.43,		// x1 wire 286
    -9.43,		// x1 wire 287
   -14.43,		// x1 wire 288
   -19.43,		// x1 wire 289
   -24.43,		// x1 wire 290
   -29.43,		// x1 wire 291
   -34.43,		// x1 wire 292
   -39.43,		// x1 wire 293
   -44.43,		// x1 wire 294
   -49.43,		// x1 wire 295
   -54.43,		// x1 wire 296
   -59.43,		// x1 wire 297
   -64.43,		// x1 wire 298
   -69.43,		// x1 wire 299
   -74.43,		// x1 wire 300
   -79.43,		// x1 wire 301
   -84.43,		// x1 wire 302
   -89.43,		// x1 wire 303
   -94.43,		// x1 wire 304
   -99.43,		// x1 wire 305
  -104.43,		// x1 wire 306
  -109.43,		// x1 wire 307
  -114.43,		// x1 wire 308
  -119.43,		// x1 wire 309
  -124.43,		// x1 wire 310
  -129.43,		// x1 wire 311
  -134.43,		// x1 wire 312
  -139.43,		// x1 wire 313
  -144.43,		// x1 wire 314
  -149.43,		// x1 wire 315
  -154.43,		// x1 wire 316
  -159.43,		// x1 wire 317
  -164.43,		// x1 wire 318
  -169.43,		// x1 wire 319
  -174.43,		// x1 wire 320
  -179.43,		// x1 wire 321
  -184.43,		// x1 wire 322
  -189.43,		// x1 wire 323
  -194.43,		// x1 wire 324
  -199.43,		// x1 wire 325
  -204.43,		// x1 wire 326
  -209.43,		// x1 wire 327
  -214.43,		// x1 wire 328
  -219.43,		// x1 wire 329
  -224.43,		// x1 wire 330
  -229.43,		// x1 wire 331
  -234.43,		// x1 wire 332
  -239.43,		// x1 wire 333
  -244.43,		// x1 wire 334
  -249.43,		// x1 wire 335
  -254.43,		// x1 wire 336
  -259.43,		// x1 wire 337
  -264.43,		// x1 wire 338
  -269.43,		// x1 wire 339
  -274.43,		// x1 wire 340
  -279.43,		// x1 wire 341
  -284.43,		// x1 wire 342
  -289.43,		// x1 wire 343
  -294.43,		// x1 wire 344
  -299.43,		// x1 wire 345
  -304.43,		// x1 wire 346
  -309.43,		// x1 wire 347
  -314.43,		// x1 wire 348
  -319.43,		// x1 wire 349
  -324.43,		// x1 wire 350
  -329.43,		// x1 wire 351
  -334.43,		// x1 wire 352
  -339.43,		// x1 wire 353
  -344.43,		// x1 wire 354
  -349.43,		// x1 wire 355
  -354.43,		// x1 wire 356
  -359.43,		// x1 wire 357
  -364.43,		// x1 wire 358
  -369.43,		// x1 wire 359
  -374.43,		// x1 wire 360
  -379.43,		// x1 wire 361
  -384.43,		// x1 wire 362
  -389.43,		// x1 wire 363
  -394.43,		// x1 wire 364
  -399.43,		// x1 wire 365
  -404.43,		// x1 wire 366
  -409.43,		// x1 wire 367
  -414.43,		// x1 wire 368
  -419.43,		// x1 wire 369
  -424.43,		// x1 wire 370
  -429.43,		// x1 wire 371
  -434.43,		// x1 wire 372
  -439.43,		// x1 wire 373
  -444.43,		// x1 wire 374
  -449.43,		// x1 wire 375
  -454.43,		// x1 wire 376
  -459.43,		// x1 wire 377
  -464.43,		// x1 wire 378
  -469.43,		// x1 wire 379
  -474.43,		// x1 wire 380
  -479.43,		// x1 wire 381
  -484.43,		// x1 wire 382
  -489.43,		// x1 wire 383
  -494.43,		// x1 wire 384
  -499.43,		// x1 wire 385
  -504.43,		// x1 wire 386
  -509.43,		// x1 wire 387
  -514.43,		// x1 wire 388
  -519.43,		// x1 wire 389
  -524.43,		// x1 wire 390
  -529.43,		// x1 wire 391
  -534.43,		// x1 wire 392
  -539.43,		// x1 wire 393
  -544.43,		// x1 wire 394
  -549.43,		// x1 wire 395
  -554.43,		// x1 wire 396
  -559.43,		// x1 wire 397
  -564.43,		// x1 wire 398
  -569.43,		// x1 wire 399
  -574.43,		// x1 wire 400
  -579.43,		// x1 wire 401
  -584.43,		// x1 wire 402
  -589.43,		// x1 wire 403
  -594.43,		// x1 wire 404
  -599.43,		// x1 wire 405
  -604.43,		// x1 wire 406
  -609.43,		// x1 wire 407
  -614.43,		// x1 wire 408
  -619.43,		// x1 wire 409
  -624.43,		// x1 wire 410
  -629.43,		// x1 wire 411
  -634.43,		// x1 wire 412
  -639.43,		// x1 wire 413
  -644.43,		// x1 wire 414
  -649.43,		// x1 wire 415
  -654.43		// x1 wire 416
};

static float
position_as1[CH_A_MAX_S1] = {
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
   766.04,		// s1 wire 80
   761.04,		// s1 wire 81
   756.04,		// s1 wire 82
   751.04,		// s1 wire 83
   746.04,		// s1 wire 84
   741.04,		// s1 wire 85
   736.04,		// s1 wire 86
   731.04,		// s1 wire 87
   726.04,		// s1 wire 88
   721.04,		// s1 wire 89
   716.04,		// s1 wire 90
   711.04,		// s1 wire 91
   706.04,		// s1 wire 92
   701.04,		// s1 wire 93
   696.04,		// s1 wire 94
   691.04,		// s1 wire 95
   686.04,		// s1 wire 96
   681.04,		// s1 wire 97
   676.04,		// s1 wire 98
   671.04,		// s1 wire 99
   666.04,		// s1 wire 100
   661.04,		// s1 wire 101
   656.04,		// s1 wire 102
   651.04,		// s1 wire 103
   646.04,		// s1 wire 104
   641.04,		// s1 wire 105
   636.04,		// s1 wire 106
   631.04,		// s1 wire 107
   626.04,		// s1 wire 108
   621.04,		// s1 wire 109
   616.04,		// s1 wire 110
   611.04,		// s1 wire 111
   606.04,		// s1 wire 112
   601.04,		// s1 wire 113
   596.04,		// s1 wire 114
   591.04,		// s1 wire 115
   586.04,		// s1 wire 116
   581.04,		// s1 wire 117
   576.04,		// s1 wire 118
   571.04,		// s1 wire 119
   566.04,		// s1 wire 120
   561.04,		// s1 wire 121
   556.04,		// s1 wire 122
   551.04,		// s1 wire 123
   546.04,		// s1 wire 124
   541.04,		// s1 wire 125
   536.04,		// s1 wire 126
   531.04,		// s1 wire 127
   526.04,		// s1 wire 128
   521.04,		// s1 wire 129
   516.04,		// s1 wire 130
   511.04,		// s1 wire 131
   506.04,		// s1 wire 132
   501.04,		// s1 wire 133
   496.04,		// s1 wire 134
   491.04,		// s1 wire 135
   486.04,		// s1 wire 136
   481.04,		// s1 wire 137
   476.04,		// s1 wire 138
   471.04,		// s1 wire 139
   466.04,		// s1 wire 140
   461.04,		// s1 wire 141
   456.04,		// s1 wire 142
   451.04,		// s1 wire 143
   446.04,		// s1 wire 144
   441.04,		// s1 wire 145
   436.04,		// s1 wire 146
   431.04,		// s1 wire 147
   426.04,		// s1 wire 148
   421.04,		// s1 wire 149
   416.04,		// s1 wire 150
   411.04,		// s1 wire 151
   406.04,		// s1 wire 152
   401.04,		// s1 wire 153
   396.04,		// s1 wire 154
   391.04,		// s1 wire 155
   386.04,		// s1 wire 156
   381.04,		// s1 wire 157
   376.04,		// s1 wire 158
   371.04,		// s1 wire 159
   366.04,		// s1 wire 160
   361.04,		// s1 wire 161
   356.04,		// s1 wire 162
   351.04,		// s1 wire 163
   346.04,		// s1 wire 164
   341.04,		// s1 wire 165
   336.04,		// s1 wire 166
   331.04,		// s1 wire 167
   326.04,		// s1 wire 168
   321.04,		// s1 wire 169
   316.04,		// s1 wire 170
   311.04,		// s1 wire 171
   306.04,		// s1 wire 172
   301.04,		// s1 wire 173
   296.04,		// s1 wire 174
   291.04,		// s1 wire 175
   286.04,		// s1 wire 176
   281.04,		// s1 wire 177
   276.04,		// s1 wire 178
   271.04,		// s1 wire 179
   266.04,		// s1 wire 180
   261.04,		// s1 wire 181
   256.04,		// s1 wire 182
   251.04,		// s1 wire 183
   246.04,		// s1 wire 184
   241.04,		// s1 wire 185
   236.04,		// s1 wire 186
   231.04,		// s1 wire 187
   226.04,		// s1 wire 188
   221.04,		// s1 wire 189
   216.04,		// s1 wire 190
   211.04,		// s1 wire 191
   206.04,		// s1 wire 192
   201.04,		// s1 wire 193
   196.04,		// s1 wire 194
   191.04,		// s1 wire 195
   186.04,		// s1 wire 196
   181.04,		// s1 wire 197
   176.04,		// s1 wire 198
   171.04,		// s1 wire 199
   166.04,		// s1 wire 200
   161.04,		// s1 wire 201
   156.04,		// s1 wire 202
   151.04,		// s1 wire 203
   146.04,		// s1 wire 204
   141.04,		// s1 wire 205
   136.04,		// s1 wire 206
   131.04,		// s1 wire 207
   126.04,		// s1 wire 208
   121.04,		// s1 wire 209
   116.04,		// s1 wire 210
   111.04,		// s1 wire 211
   106.04,		// s1 wire 212
   101.04,		// s1 wire 213
    96.04,		// s1 wire 214
    91.04,		// s1 wire 215
    86.04,		// s1 wire 216
    81.04,		// s1 wire 217
    76.04,		// s1 wire 218
    71.04,		// s1 wire 219
    66.04,		// s1 wire 220
    61.04,		// s1 wire 221
    56.04,		// s1 wire 222
    51.04,		// s1 wire 223
    46.04,		// s1 wire 224
    41.04,		// s1 wire 225
    36.04,		// s1 wire 226
    31.04,		// s1 wire 227
    26.04,		// s1 wire 228
    21.04,		// s1 wire 229
    16.04,		// s1 wire 230
    11.04,		// s1 wire 231
     6.04,		// s1 wire 232
     1.04,		// s1 wire 233
    -3.96,		// s1 wire 234
    -8.96,		// s1 wire 235
   -13.96,		// s1 wire 236
   -18.96,		// s1 wire 237
   -23.96,		// s1 wire 238
   -28.96,		// s1 wire 239
   -33.96,		// s1 wire 240
   -38.96,		// s1 wire 241
   -43.96,		// s1 wire 242
   -48.96,		// s1 wire 243
   -53.96,		// s1 wire 244
   -58.96,		// s1 wire 245
   -63.96,		// s1 wire 246
   -68.96,		// s1 wire 247
   -73.96,		// s1 wire 248
   -78.96,		// s1 wire 249
   -83.96,		// s1 wire 250
   -88.96,		// s1 wire 251
   -93.96,		// s1 wire 252
   -98.96,		// s1 wire 253
  -103.96,		// s1 wire 254
  -108.96,		// s1 wire 255
  -113.96,		// s1 wire 256
  -118.96,		// s1 wire 257
  -123.96,		// s1 wire 258
  -128.96,		// s1 wire 259
  -133.96,		// s1 wire 260
  -138.96,		// s1 wire 261
  -143.96,		// s1 wire 262
  -148.96,		// s1 wire 263
  -153.96,		// s1 wire 264
  -158.96,		// s1 wire 265
  -163.96,		// s1 wire 266
  -168.96,		// s1 wire 267
  -173.96,		// s1 wire 268
  -178.96,		// s1 wire 269
  -183.96,		// s1 wire 270
  -188.96,		// s1 wire 271
  -193.96,		// s1 wire 272
  -198.96,		// s1 wire 273
  -203.96,		// s1 wire 274
  -208.96,		// s1 wire 275
  -213.96,		// s1 wire 276
  -218.96,		// s1 wire 277
  -223.96,		// s1 wire 278
  -228.96,		// s1 wire 279
  -233.96,		// s1 wire 280
  -238.96,		// s1 wire 281
  -243.96,		// s1 wire 282
  -248.96,		// s1 wire 283
  -253.96,		// s1 wire 284
  -258.96,		// s1 wire 285
  -263.96,		// s1 wire 286
  -268.96,		// s1 wire 287
  -273.96,		// s1 wire 288
  -278.96,		// s1 wire 289
  -283.96,		// s1 wire 290
  -288.96,		// s1 wire 291
  -293.96,		// s1 wire 292
  -298.96,		// s1 wire 293
  -303.96,		// s1 wire 294
  -308.96,		// s1 wire 295
  -313.96,		// s1 wire 296
  -318.96,		// s1 wire 297
  -323.96,		// s1 wire 298
  -328.96,		// s1 wire 299
  -333.96,		// s1 wire 300
  -338.96,		// s1 wire 301
  -343.96,		// s1 wire 302
  -348.96,		// s1 wire 303
  -353.96,		// s1 wire 304
  -358.96,		// s1 wire 305
  -363.96,		// s1 wire 306
  -368.96,		// s1 wire 307
  -373.96,		// s1 wire 308
  -378.96,		// s1 wire 309
  -383.96,		// s1 wire 310
  -388.96,		// s1 wire 311
  -393.96,		// s1 wire 312
  -398.96,		// s1 wire 313
  -403.96,		// s1 wire 314
  -408.96,		// s1 wire 315
  -413.96,		// s1 wire 316
  -418.96,		// s1 wire 317
  -423.96,		// s1 wire 318
  -428.96,		// s1 wire 319
  -433.96,		// s1 wire 320
  -438.96,		// s1 wire 321
  -443.96,		// s1 wire 322
  -448.96,		// s1 wire 323
  -453.96,		// s1 wire 324
  -458.96,		// s1 wire 325
  -463.96,		// s1 wire 326
  -468.96,		// s1 wire 327
  -473.96,		// s1 wire 328
  -478.96,		// s1 wire 329
  -483.96,		// s1 wire 330
  -488.96,		// s1 wire 331
  -493.96,		// s1 wire 332
  -498.96,		// s1 wire 333
  -503.96,		// s1 wire 334
  -508.96,		// s1 wire 335
  -513.96,		// s1 wire 336
  -518.96,		// s1 wire 337
  -523.96,		// s1 wire 338
  -528.96,		// s1 wire 339
  -533.96,		// s1 wire 340
  -538.96,		// s1 wire 341
  -543.96,		// s1 wire 342
  -548.96,		// s1 wire 343
  -553.96,		// s1 wire 344
  -558.96,		// s1 wire 345
  -563.96,		// s1 wire 346
  -568.96,		// s1 wire 347
  -573.96,		// s1 wire 348
  -578.96,		// s1 wire 349
  -583.96,		// s1 wire 350
  -588.96,		// s1 wire 351
  -593.96		// s1 wire 352
};

static float
position_ax2[CH_A_MAX_X2] = {
  1772.57,		// x2 wire 1
  1767.57,		// x2 wire 2
  1762.57,		// x2 wire 3
  1757.57,		// x2 wire 4
  1752.57,		// x2 wire 5
  1747.57,		// x2 wire 6
  1742.57,		// x2 wire 7
  1737.57,		// x2 wire 8
  1732.57,		// x2 wire 9
  1727.57,		// x2 wire 10
  1722.57,		// x2 wire 11
  1717.57,		// x2 wire 12
  1712.57,		// x2 wire 13
  1707.57,		// x2 wire 14
  1702.57,		// x2 wire 15
  1697.57,		// x2 wire 16
  1692.57,		// x2 wire 17
  1687.57,		// x2 wire 18
  1682.57,		// x2 wire 19
  1677.57,		// x2 wire 20
  1672.57,		// x2 wire 21
  1667.57,		// x2 wire 22
  1662.57,		// x2 wire 23
  1657.57,		// x2 wire 24
  1652.57,		// x2 wire 25
  1647.57,		// x2 wire 26
  1642.57,		// x2 wire 27
  1637.57,		// x2 wire 28
  1632.57,		// x2 wire 29
  1627.57,		// x2 wire 30
  1622.57,		// x2 wire 31
  1617.57,		// x2 wire 32
  1612.57,		// x2 wire 33
  1607.57,		// x2 wire 34
  1602.57,		// x2 wire 35
  1597.57,		// x2 wire 36
  1592.57,		// x2 wire 37
  1587.57,		// x2 wire 38
  1582.57,		// x2 wire 39
  1577.57,		// x2 wire 40
  1572.57,		// x2 wire 41
  1567.57,		// x2 wire 42
  1562.57,		// x2 wire 43
  1557.57,		// x2 wire 44
  1552.57,		// x2 wire 45
  1547.57,		// x2 wire 46
  1542.57,		// x2 wire 47
  1537.57,		// x2 wire 48
  1532.57,		// x2 wire 49
  1527.57,		// x2 wire 50
  1522.57,		// x2 wire 51
  1517.57,		// x2 wire 52
  1512.57,		// x2 wire 53
  1507.57,		// x2 wire 54
  1502.57,		// x2 wire 55
  1497.57,		// x2 wire 56
  1492.57,		// x2 wire 57
  1487.57,		// x2 wire 58
  1482.57,		// x2 wire 59
  1477.57,		// x2 wire 60
  1472.57,		// x2 wire 61
  1467.57,		// x2 wire 62
  1462.57,		// x2 wire 63
  1457.57,		// x2 wire 64
  1452.57,		// x2 wire 65
  1447.57,		// x2 wire 66
  1442.57,		// x2 wire 67
  1437.57,		// x2 wire 68
  1432.57,		// x2 wire 69
  1427.57,		// x2 wire 70
  1422.57,		// x2 wire 71
  1417.57,		// x2 wire 72
  1412.57,		// x2 wire 73
  1407.57,		// x2 wire 74
  1402.57,		// x2 wire 75
  1397.57,		// x2 wire 76
  1392.57,		// x2 wire 77
  1387.57,		// x2 wire 78
  1382.57,		// x2 wire 79
  1377.57,		// x2 wire 80
  1372.57,		// x2 wire 81
  1367.57,		// x2 wire 82
  1362.57,		// x2 wire 83
  1357.57,		// x2 wire 84
  1352.57,		// x2 wire 85
  1347.57,		// x2 wire 86
  1342.57,		// x2 wire 87
  1337.57,		// x2 wire 88
  1332.57,		// x2 wire 89
  1327.57,		// x2 wire 90
  1322.57,		// x2 wire 91
  1317.57,		// x2 wire 92
  1312.57,		// x2 wire 93
  1307.57,		// x2 wire 94
  1302.57,		// x2 wire 95
  1297.57,		// x2 wire 96
  1292.57,		// x2 wire 97
  1287.57,		// x2 wire 98
  1282.57,		// x2 wire 99
  1277.57,		// x2 wire 100
  1272.57,		// x2 wire 101
  1267.57,		// x2 wire 102
  1262.57,		// x2 wire 103
  1257.57,		// x2 wire 104
  1252.57,		// x2 wire 105
  1247.57,		// x2 wire 106
  1242.57,		// x2 wire 107
  1237.57,		// x2 wire 108
  1232.57,		// x2 wire 109
  1227.57,		// x2 wire 110
  1222.57,		// x2 wire 111
  1217.57,		// x2 wire 112
  1212.57,		// x2 wire 113
  1207.57,		// x2 wire 114
  1202.57,		// x2 wire 115
  1197.57,		// x2 wire 116
  1192.57,		// x2 wire 117
  1187.57,		// x2 wire 118
  1182.57,		// x2 wire 119
  1177.57,		// x2 wire 120
  1172.57,		// x2 wire 121
  1167.57,		// x2 wire 122
  1162.57,		// x2 wire 123
  1157.57,		// x2 wire 124
  1152.57,		// x2 wire 125
  1147.57,		// x2 wire 126
  1142.57,		// x2 wire 127
  1137.57,		// x2 wire 128
  1132.57,		// x2 wire 129
  1127.57,		// x2 wire 130
  1122.57,		// x2 wire 131
  1117.57,		// x2 wire 132
  1112.57,		// x2 wire 133
  1107.57,		// x2 wire 134
  1102.57,		// x2 wire 135
  1097.57,		// x2 wire 136
  1092.57,		// x2 wire 137
  1087.57,		// x2 wire 138
  1082.57,		// x2 wire 139
  1077.57,		// x2 wire 140
  1072.57,		// x2 wire 141
  1067.57,		// x2 wire 142
  1062.57,		// x2 wire 143
  1057.57,		// x2 wire 144
  1052.57,		// x2 wire 145
  1047.57,		// x2 wire 146
  1042.57,		// x2 wire 147
  1037.57,		// x2 wire 148
  1032.57,		// x2 wire 149
  1027.57,		// x2 wire 150
  1022.57,		// x2 wire 151
  1017.57,		// x2 wire 152
  1012.57,		// x2 wire 153
  1007.57,		// x2 wire 154
  1002.57,		// x2 wire 155
   997.57,		// x2 wire 156
   992.57,		// x2 wire 157
   987.57,		// x2 wire 158
   982.57,		// x2 wire 159
   977.57,		// x2 wire 160
   972.57,		// x2 wire 161
   967.57,		// x2 wire 162
   962.57,		// x2 wire 163
   957.57,		// x2 wire 164
   952.57,		// x2 wire 165
   947.57,		// x2 wire 166
   942.57,		// x2 wire 167
   937.57,		// x2 wire 168
   932.57,		// x2 wire 169
   927.57,		// x2 wire 170
   922.57,		// x2 wire 171
   917.57,		// x2 wire 172
   912.57,		// x2 wire 173
   907.57,		// x2 wire 174
   902.57,		// x2 wire 175
   897.57,		// x2 wire 176
   892.57,		// x2 wire 177
   887.57,		// x2 wire 178
   882.57,		// x2 wire 179
   877.57,		// x2 wire 180
   872.57,		// x2 wire 181
   867.57,		// x2 wire 182
   862.57,		// x2 wire 183
   857.57,		// x2 wire 184
   852.57,		// x2 wire 185
   847.57,		// x2 wire 186
   842.57,		// x2 wire 187
   837.57,		// x2 wire 188
   832.57,		// x2 wire 189
   827.57,		// x2 wire 190
   822.57,		// x2 wire 191
   817.57,		// x2 wire 192
   812.57,		// x2 wire 193
   807.57,		// x2 wire 194
   802.57,		// x2 wire 195
   797.57,		// x2 wire 196
   792.57,		// x2 wire 197
   787.57,		// x2 wire 198
   782.57,		// x2 wire 199
   777.57,		// x2 wire 200
   772.57,		// x2 wire 201
   767.57,		// x2 wire 202
   762.57,		// x2 wire 203
   757.57,		// x2 wire 204
   752.57,		// x2 wire 205
   747.57,		// x2 wire 206
   742.57,		// x2 wire 207
   737.57,		// x2 wire 208
   732.57,		// x2 wire 209
   727.57,		// x2 wire 210
   722.57,		// x2 wire 211
   717.57,		// x2 wire 212
   712.57,		// x2 wire 213
   707.57,		// x2 wire 214
   702.57,		// x2 wire 215
   697.57,		// x2 wire 216
   692.57,		// x2 wire 217
   687.57,		// x2 wire 218
   682.57,		// x2 wire 219
   677.57,		// x2 wire 220
   672.57,		// x2 wire 221
   667.57,		// x2 wire 222
   662.57,		// x2 wire 223
   657.57,		// x2 wire 224
   652.57,		// x2 wire 225
   647.57,		// x2 wire 226
   642.57,		// x2 wire 227
   637.57,		// x2 wire 228
   632.57,		// x2 wire 229
   627.57,		// x2 wire 230
   622.57,		// x2 wire 231
   617.57,		// x2 wire 232
   612.57,		// x2 wire 233
   607.57,		// x2 wire 234
   602.57,		// x2 wire 235
   597.57,		// x2 wire 236
   592.57,		// x2 wire 237
   587.57,		// x2 wire 238
   582.57,		// x2 wire 239
   577.57,		// x2 wire 240
   572.57,		// x2 wire 241
   567.57,		// x2 wire 242
   562.57,		// x2 wire 243
   557.57,		// x2 wire 244
   552.57,		// x2 wire 245
   547.57,		// x2 wire 246
   542.57,		// x2 wire 247
   537.57,		// x2 wire 248
   532.57,		// x2 wire 249
   527.57,		// x2 wire 250
   522.57,		// x2 wire 251
   517.57,		// x2 wire 252
   512.57,		// x2 wire 253
   507.57,		// x2 wire 254
   502.57,		// x2 wire 255
   497.57,		// x2 wire 256
   492.57,		// x2 wire 257
   487.57,		// x2 wire 258
   482.57,		// x2 wire 259
   477.57,		// x2 wire 260
   472.57,		// x2 wire 261
   467.57,		// x2 wire 262
   462.57,		// x2 wire 263
   457.57,		// x2 wire 264
   452.57,		// x2 wire 265
   447.57,		// x2 wire 266
   442.57,		// x2 wire 267
   437.57,		// x2 wire 268
   432.57,		// x2 wire 269
   427.57,		// x2 wire 270
   422.57,		// x2 wire 271
   417.57,		// x2 wire 272
   412.57,		// x2 wire 273
   407.57,		// x2 wire 274
   402.57,		// x2 wire 275
   397.57,		// x2 wire 276
   392.57,		// x2 wire 277
   387.57,		// x2 wire 278
   382.57,		// x2 wire 279
   377.57,		// x2 wire 280
   372.57,		// x2 wire 281
   367.57,		// x2 wire 282
   362.57,		// x2 wire 283
   357.57,		// x2 wire 284
   352.57,		// x2 wire 285
   347.57,		// x2 wire 286
   342.57,		// x2 wire 287
   337.57,		// x2 wire 288
   332.57,		// x2 wire 289
   327.57,		// x2 wire 290
   322.57,		// x2 wire 291
   317.57,		// x2 wire 292
   312.57,		// x2 wire 293
   307.57,		// x2 wire 294
   302.57,		// x2 wire 295
   297.57,		// x2 wire 296
   292.57,		// x2 wire 297
   287.57,		// x2 wire 298
   282.57,		// x2 wire 299
   277.57,		// x2 wire 300
   272.57,		// x2 wire 301
   267.57,		// x2 wire 302
   262.57,		// x2 wire 303
   257.57,		// x2 wire 304
   252.57,		// x2 wire 305
   247.57,		// x2 wire 306
   242.57,		// x2 wire 307
   237.57,		// x2 wire 308
   232.57,		// x2 wire 309
   227.57,		// x2 wire 310
   222.57,		// x2 wire 311
   217.57,		// x2 wire 312
   212.57,		// x2 wire 313
   207.57,		// x2 wire 314
   202.57,		// x2 wire 315
   197.57,		// x2 wire 316
   192.57,		// x2 wire 317
   187.57,		// x2 wire 318
   182.57,		// x2 wire 319
   177.57,		// x2 wire 320
   172.57,		// x2 wire 321
   167.57,		// x2 wire 322
   162.57,		// x2 wire 323
   157.57,		// x2 wire 324
   152.57,		// x2 wire 325
   147.57,		// x2 wire 326
   142.57,		// x2 wire 327
   137.57,		// x2 wire 328
   132.57,		// x2 wire 329
   127.57,		// x2 wire 330
   122.57,		// x2 wire 331
   117.57,		// x2 wire 332
   112.57,		// x2 wire 333
   107.57,		// x2 wire 334
   102.57,		// x2 wire 335
    97.57,		// x2 wire 336
    92.57,		// x2 wire 337
    87.57,		// x2 wire 338
    82.57,		// x2 wire 339
    77.57,		// x2 wire 340
    72.57,		// x2 wire 341
    67.57,		// x2 wire 342
    62.57,		// x2 wire 343
    57.57,		// x2 wire 344
    52.57,		// x2 wire 345
    47.57,		// x2 wire 346
    42.57,		// x2 wire 347
    37.57,		// x2 wire 348
    32.57,		// x2 wire 349
    27.57,		// x2 wire 350
    22.57,		// x2 wire 351
    17.57,		// x2 wire 352
    12.57,		// x2 wire 353
     7.57,		// x2 wire 354
     2.57,		// x2 wire 355
    -2.43,		// x2 wire 356
    -7.43,		// x2 wire 357
   -12.43,		// x2 wire 358
   -17.43,		// x2 wire 359
   -22.43,		// x2 wire 360
   -27.43,		// x2 wire 361
   -32.43,		// x2 wire 362
   -37.43,		// x2 wire 363
   -42.43,		// x2 wire 364
   -47.43,		// x2 wire 365
   -52.43,		// x2 wire 366
   -57.43,		// x2 wire 367
   -62.43,		// x2 wire 368
   -67.43,		// x2 wire 369
   -72.43,		// x2 wire 370
   -77.43,		// x2 wire 371
   -82.43,		// x2 wire 372
   -87.43,		// x2 wire 373
   -92.43,		// x2 wire 374
   -97.43,		// x2 wire 375
  -102.43,		// x2 wire 376
  -107.43,		// x2 wire 377
  -112.43,		// x2 wire 378
  -117.43,		// x2 wire 379
  -122.43,		// x2 wire 380
  -127.43,		// x2 wire 381
  -132.43,		// x2 wire 382
  -137.43,		// x2 wire 383
  -142.43,		// x2 wire 384
  -147.43,		// x2 wire 385
  -152.43,		// x2 wire 386
  -157.43,		// x2 wire 387
  -162.43,		// x2 wire 388
  -167.43,		// x2 wire 389
  -172.43,		// x2 wire 390
  -177.43,		// x2 wire 391
  -182.43,		// x2 wire 392
  -187.43,		// x2 wire 393
  -192.43,		// x2 wire 394
  -197.43,		// x2 wire 395
  -202.43,		// x2 wire 396
  -207.43,		// x2 wire 397
  -212.43,		// x2 wire 398
  -217.43,		// x2 wire 399
  -222.43,		// x2 wire 400
  -227.43,		// x2 wire 401
  -232.43,		// x2 wire 402
  -237.43,		// x2 wire 403
  -242.43,		// x2 wire 404
  -247.43,		// x2 wire 405
  -252.43,		// x2 wire 406
  -257.43,		// x2 wire 407
  -262.43,		// x2 wire 408
  -267.43,		// x2 wire 409
  -272.43,		// x2 wire 410
  -277.43,		// x2 wire 411
  -282.43,		// x2 wire 412
  -287.43,		// x2 wire 413
  -292.43,		// x2 wire 414
  -297.43,		// x2 wire 415
  -302.43		// x2 wire 416
};

static float
position_as2[CH_A_MAX_S2] = {
  1430.69,		// s2 wire 1
  1425.69,		// s2 wire 2
  1420.69,		// s2 wire 3
  1415.69,		// s2 wire 4
  1410.69,		// s2 wire 5
  1405.69,		// s2 wire 6
  1400.69,		// s2 wire 7
  1395.69,		// s2 wire 8
  1390.69,		// s2 wire 9
  1385.69,		// s2 wire 10
  1380.69,		// s2 wire 11
  1375.69,		// s2 wire 12
  1370.69,		// s2 wire 13
  1365.69,		// s2 wire 14
  1360.69,		// s2 wire 15
  1355.69,		// s2 wire 16
  1350.69,		// s2 wire 17
  1345.69,		// s2 wire 18
  1340.69,		// s2 wire 19
  1335.69,		// s2 wire 20
  1330.69,		// s2 wire 21
  1325.69,		// s2 wire 22
  1320.69,		// s2 wire 23
  1315.69,		// s2 wire 24
  1310.69,		// s2 wire 25
  1305.69,		// s2 wire 26
  1300.69,		// s2 wire 27
  1295.69,		// s2 wire 28
  1290.69,		// s2 wire 29
  1285.69,		// s2 wire 30
  1280.69,		// s2 wire 31
  1275.69,		// s2 wire 32
  1270.69,		// s2 wire 33
  1265.69,		// s2 wire 34
  1260.69,		// s2 wire 35
  1255.69,		// s2 wire 36
  1250.69,		// s2 wire 37
  1245.69,		// s2 wire 38
  1240.69,		// s2 wire 39
  1235.69,		// s2 wire 40
  1230.69,		// s2 wire 41
  1225.69,		// s2 wire 42
  1220.69,		// s2 wire 43
  1215.69,		// s2 wire 44
  1210.69,		// s2 wire 45
  1205.69,		// s2 wire 46
  1200.69,		// s2 wire 47
  1195.69,		// s2 wire 48
  1190.69,		// s2 wire 49
  1185.69,		// s2 wire 50
  1180.69,		// s2 wire 51
  1175.69,		// s2 wire 52
  1170.69,		// s2 wire 53
  1165.69,		// s2 wire 54
  1160.69,		// s2 wire 55
  1155.69,		// s2 wire 56
  1150.69,		// s2 wire 57
  1145.69,		// s2 wire 58
  1140.69,		// s2 wire 59
  1135.69,		// s2 wire 60
  1130.69,		// s2 wire 61
  1125.69,		// s2 wire 62
  1120.69,		// s2 wire 63
  1115.69,		// s2 wire 64
  1110.69,		// s2 wire 65
  1105.69,		// s2 wire 66
  1100.69,		// s2 wire 67
  1095.69,		// s2 wire 68
  1090.69,		// s2 wire 69
  1085.69,		// s2 wire 70
  1080.69,		// s2 wire 71
  1075.69,		// s2 wire 72
  1070.69,		// s2 wire 73
  1065.69,		// s2 wire 74
  1060.69,		// s2 wire 75
  1055.69,		// s2 wire 76
  1050.69,		// s2 wire 77
  1045.69,		// s2 wire 78
  1040.69,		// s2 wire 79
  1035.69,		// s2 wire 80
  1030.69,		// s2 wire 81
  1025.69,		// s2 wire 82
  1020.69,		// s2 wire 83
  1015.69,		// s2 wire 84
  1010.69,		// s2 wire 85
  1005.69,		// s2 wire 86
  1000.69,		// s2 wire 87
   995.69,		// s2 wire 88
   990.69,		// s2 wire 89
   985.69,		// s2 wire 90
   980.69,		// s2 wire 91
   975.69,		// s2 wire 92
   970.69,		// s2 wire 93
   965.69,		// s2 wire 94
   960.69,		// s2 wire 95
   955.69,		// s2 wire 96
   950.69,		// s2 wire 97
   945.69,		// s2 wire 98
   940.69,		// s2 wire 99
   935.69,		// s2 wire 100
   930.69,		// s2 wire 101
   925.69,		// s2 wire 102
   920.69,		// s2 wire 103
   915.69,		// s2 wire 104
   910.69,		// s2 wire 105
   905.69,		// s2 wire 106
   900.69,		// s2 wire 107
   895.69,		// s2 wire 108
   890.69,		// s2 wire 109
   885.69,		// s2 wire 110
   880.69,		// s2 wire 111
   875.69,		// s2 wire 112
   870.69,		// s2 wire 113
   865.69,		// s2 wire 114
   860.69,		// s2 wire 115
   855.69,		// s2 wire 116
   850.69,		// s2 wire 117
   845.69,		// s2 wire 118
   840.69,		// s2 wire 119
   835.69,		// s2 wire 120
   830.69,		// s2 wire 121
   825.69,		// s2 wire 122
   820.69,		// s2 wire 123
   815.69,		// s2 wire 124
   810.69,		// s2 wire 125
   805.69,		// s2 wire 126
   800.69,		// s2 wire 127
   795.69,		// s2 wire 128
   790.69,		// s2 wire 129
   785.69,		// s2 wire 130
   780.69,		// s2 wire 131
   775.69,		// s2 wire 132
   770.69,		// s2 wire 133
   765.69,		// s2 wire 134
   760.69,		// s2 wire 135
   755.69,		// s2 wire 136
   750.69,		// s2 wire 137
   745.69,		// s2 wire 138
   740.69,		// s2 wire 139
   735.69,		// s2 wire 140
   730.69,		// s2 wire 141
   725.69,		// s2 wire 142
   720.69,		// s2 wire 143
   715.69,		// s2 wire 144
   710.69,		// s2 wire 145
   705.69,		// s2 wire 146
   700.69,		// s2 wire 147
   695.69,		// s2 wire 148
   690.69,		// s2 wire 149
   685.69,		// s2 wire 150
   680.69,		// s2 wire 151
   675.69,		// s2 wire 152
   670.69,		// s2 wire 153
   665.69,		// s2 wire 154
   660.69,		// s2 wire 155
   655.69,		// s2 wire 156
   650.69,		// s2 wire 157
   645.69,		// s2 wire 158
   640.69,		// s2 wire 159
   635.69,		// s2 wire 160
   630.69,		// s2 wire 161
   625.69,		// s2 wire 162
   620.69,		// s2 wire 163
   615.69,		// s2 wire 164
   610.69,		// s2 wire 165
   605.69,		// s2 wire 166
   600.69,		// s2 wire 167
   595.69,		// s2 wire 168
   590.69,		// s2 wire 169
   585.69,		// s2 wire 170
   580.69,		// s2 wire 171
   575.69,		// s2 wire 172
   570.69,		// s2 wire 173
   565.69,		// s2 wire 174
   560.69,		// s2 wire 175
   555.69,		// s2 wire 176
   550.69,		// s2 wire 177
   545.69,		// s2 wire 178
   540.69,		// s2 wire 179
   535.69,		// s2 wire 180
   530.69,		// s2 wire 181
   525.69,		// s2 wire 182
   520.69,		// s2 wire 183
   515.69,		// s2 wire 184
   510.69,		// s2 wire 185
   505.69,		// s2 wire 186
   500.69,		// s2 wire 187
   495.69,		// s2 wire 188
   490.69,		// s2 wire 189
   485.69,		// s2 wire 190
   480.69,		// s2 wire 191
   475.69,		// s2 wire 192
   470.69,		// s2 wire 193
   465.69,		// s2 wire 194
   460.69,		// s2 wire 195
   455.69,		// s2 wire 196
   450.69,		// s2 wire 197
   445.69,		// s2 wire 198
   440.69,		// s2 wire 199
   435.69,		// s2 wire 200
   430.69,		// s2 wire 201
   425.69,		// s2 wire 202
   420.69,		// s2 wire 203
   415.69,		// s2 wire 204
   410.69,		// s2 wire 205
   405.69,		// s2 wire 206
   400.69,		// s2 wire 207
   395.69,		// s2 wire 208
   390.69,		// s2 wire 209
   385.69,		// s2 wire 210
   380.69,		// s2 wire 211
   375.69,		// s2 wire 212
   370.69,		// s2 wire 213
   365.69,		// s2 wire 214
   360.69,		// s2 wire 215
   355.69,		// s2 wire 216
   350.69,		// s2 wire 217
   345.69,		// s2 wire 218
   340.69,		// s2 wire 219
   335.69,		// s2 wire 220
   330.69,		// s2 wire 221
   325.69,		// s2 wire 222
   320.69,		// s2 wire 223
   315.69,		// s2 wire 224
   310.69,		// s2 wire 225
   305.69,		// s2 wire 226
   300.69,		// s2 wire 227
   295.69,		// s2 wire 228
   290.69,		// s2 wire 229
   285.69,		// s2 wire 230
   280.69,		// s2 wire 231
   275.69,		// s2 wire 232
   270.69,		// s2 wire 233
   265.69,		// s2 wire 234
   260.69,		// s2 wire 235
   255.69,		// s2 wire 236
   250.69,		// s2 wire 237
   245.69,		// s2 wire 238
   240.69,		// s2 wire 239
   235.69,		// s2 wire 240
   230.69,		// s2 wire 241
   225.69,		// s2 wire 242
   220.69,		// s2 wire 243
   215.69,		// s2 wire 244
   210.69,		// s2 wire 245
   205.69,		// s2 wire 246
   200.69,		// s2 wire 247
   195.69,		// s2 wire 248
   190.69,		// s2 wire 249
   185.69,		// s2 wire 250
   180.69,		// s2 wire 251
   175.69,		// s2 wire 252
   170.69,		// s2 wire 253
   165.69,		// s2 wire 254
   160.69,		// s2 wire 255
   155.69,		// s2 wire 256
   150.69,		// s2 wire 257
   145.69,		// s2 wire 258
   140.69,		// s2 wire 259
   135.69,		// s2 wire 260
   130.69,		// s2 wire 261
   125.69,		// s2 wire 262
   120.69,		// s2 wire 263
   115.69,		// s2 wire 264
   110.69,		// s2 wire 265
   105.69,		// s2 wire 266
   100.69,		// s2 wire 267
    95.69,		// s2 wire 268
    90.69,		// s2 wire 269
    85.69,		// s2 wire 270
    80.69,		// s2 wire 271
    75.69,		// s2 wire 272
    70.69,		// s2 wire 273
    65.69,		// s2 wire 274
    60.69,		// s2 wire 275
    55.69,		// s2 wire 276
    50.69,		// s2 wire 277
    45.69,		// s2 wire 278
    40.69,		// s2 wire 279
    35.69,		// s2 wire 280
    30.69,		// s2 wire 281
    25.69,		// s2 wire 282
    20.69,		// s2 wire 283
    15.69,		// s2 wire 284
    10.69,		// s2 wire 285
     5.69,		// s2 wire 286
     0.69,		// s2 wire 287
    -4.31,		// s2 wire 288
    -9.31,		// s2 wire 289
   -14.31,		// s2 wire 290
   -19.31,		// s2 wire 291
   -24.31,		// s2 wire 292
   -29.31,		// s2 wire 293
   -34.31,		// s2 wire 294
   -39.31,		// s2 wire 295
   -44.31,		// s2 wire 296
   -49.31,		// s2 wire 297
   -54.31,		// s2 wire 298
   -59.31,		// s2 wire 299
   -64.31,		// s2 wire 300
   -69.31,		// s2 wire 301
   -74.31,		// s2 wire 302
   -79.31,		// s2 wire 303
   -84.31,		// s2 wire 304
   -89.31,		// s2 wire 305
   -94.31,		// s2 wire 306
   -99.31,		// s2 wire 307
  -104.31,		// s2 wire 308
  -109.31,		// s2 wire 309
  -114.31,		// s2 wire 310
  -119.31,		// s2 wire 311
  -124.31,		// s2 wire 312
  -129.31,		// s2 wire 313
  -134.31,		// s2 wire 314
  -139.31,		// s2 wire 315
  -144.31,		// s2 wire 316
  -149.31,		// s2 wire 317
  -154.31,		// s2 wire 318
  -159.31,		// s2 wire 319
  -164.31,		// s2 wire 320
  -169.31,		// s2 wire 321
  -174.31,		// s2 wire 322
  -179.31,		// s2 wire 323
  -184.31,		// s2 wire 324
  -189.31,		// s2 wire 325
  -194.31,		// s2 wire 326
  -199.31,		// s2 wire 327
  -204.31,		// s2 wire 328
  -209.31,		// s2 wire 329
  -214.31,		// s2 wire 330
  -219.31,		// s2 wire 331
  -224.31,		// s2 wire 332
  -229.31,		// s2 wire 333
  -234.31,		// s2 wire 334
  -239.31,		// s2 wire 335
  -244.31,		// s2 wire 336
  -249.31,		// s2 wire 337
  -254.31,		// s2 wire 338
  -259.31,		// s2 wire 339
  -264.31,		// s2 wire 340
  -269.31,		// s2 wire 341
  -274.31,		// s2 wire 342
  -279.31,		// s2 wire 343
  -284.31,		// s2 wire 344
  -289.31,		// s2 wire 345
  -294.31,		// s2 wire 346
  -299.31,		// s2 wire 347
  -304.31,		// s2 wire 348
  -309.31,		// s2 wire 349
  -314.31,		// s2 wire 350
  -319.31,		// s2 wire 351
  -324.31		// s2 wire 352
};

vdcPosOffs vdcPosOffsA = {
  { position_ax1, offset_ax1, CH_A_MAX_X1 },
  { position_as1, offset_as1, CH_A_MAX_S1 },
  { position_ax2, offset_ax2, CH_A_MAX_X2 },
  { position_as2, offset_as2, CH_A_MAX_S2 }
};

#endif
