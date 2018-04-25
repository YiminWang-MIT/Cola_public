//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/chinitC.h,v 2.9 1996-12-12 15:31:33 distler Exp $
//
// INIT file fuer die chamber++ library.
// Offset und Position fuer die Draehte
// von allen vier Ebenen der Driftkammern
// von Spektrometer C.
//

#ifndef __chinitC__
#define __chinitC__

const short CH_C_MAX_X1		= 448;
const short CH_C_MAX_S1		= 352;
const short CH_C_MAX_X2		= 448;
const short CH_C_MAX_S2		= 352;

const short CH_C_OFFSET_X1	= 2494;
const short CH_C_OFFSET_S1	= 2497;
const short CH_C_OFFSET_X2	= 2495;
const short CH_C_OFFSET_S2	= 2491;

const double CH_C_ANGLE		= 39.83;
const short  CH_C_FULLRANGE	= 16384;
const double CH_C_TIMETOLENGTH	= 0.25 * 0.052;

static short
offset_cx1[CH_C_MAX_X1] = {
  CH_C_OFFSET_X1,			// wire 1
  CH_C_OFFSET_X1,			// wire 2
  CH_C_OFFSET_X1,			// wire 3
  CH_C_OFFSET_X1,			// wire 4
  CH_C_OFFSET_X1,			// wire 5
  CH_C_OFFSET_X1,			// wire 6
  CH_C_OFFSET_X1,			// wire 7
  CH_C_OFFSET_X1,			// wire 8
  CH_C_OFFSET_X1,			// wire 9
  CH_C_OFFSET_X1,			// wire 10
  CH_C_OFFSET_X1,			// wire 11
  CH_C_OFFSET_X1,			// wire 12
  CH_C_OFFSET_X1,			// wire 13
  CH_C_OFFSET_X1,			// wire 14
  CH_C_OFFSET_X1,			// wire 15
  CH_C_OFFSET_X1,			// wire 16
  CH_C_OFFSET_X1,			// wire 17
  CH_C_OFFSET_X1,			// wire 18
  CH_C_OFFSET_X1,			// wire 19
  CH_C_OFFSET_X1,			// wire 20
  CH_C_OFFSET_X1,			// wire 21
  CH_C_OFFSET_X1,			// wire 22
  CH_C_OFFSET_X1,			// wire 23
  CH_C_OFFSET_X1,			// wire 24
  CH_C_OFFSET_X1,			// wire 25
  CH_C_OFFSET_X1,			// wire 26
  CH_C_OFFSET_X1,			// wire 27
  CH_C_OFFSET_X1,			// wire 28
  CH_C_OFFSET_X1,			// wire 29
  CH_C_OFFSET_X1,			// wire 30
  CH_C_OFFSET_X1,			// wire 31
  CH_C_OFFSET_X1,			// wire 32
  CH_C_OFFSET_X1,			// wire 33
  CH_C_OFFSET_X1,			// wire 34
  CH_C_OFFSET_X1,			// wire 35
  CH_C_OFFSET_X1,			// wire 36
  CH_C_OFFSET_X1,			// wire 37
  CH_C_OFFSET_X1,			// wire 38
  CH_C_OFFSET_X1,			// wire 39
  CH_C_OFFSET_X1,			// wire 40
  CH_C_OFFSET_X1,			// wire 41
  CH_C_OFFSET_X1,			// wire 42
  CH_C_OFFSET_X1,			// wire 43
  CH_C_OFFSET_X1,			// wire 44
  CH_C_OFFSET_X1,			// wire 45
  CH_C_OFFSET_X1,			// wire 46
  CH_C_OFFSET_X1,			// wire 47
  CH_C_OFFSET_X1,			// wire 48
  CH_C_OFFSET_X1,			// wire 49
  CH_C_OFFSET_X1,			// wire 50
  CH_C_OFFSET_X1,			// wire 51
  CH_C_OFFSET_X1,			// wire 52
  CH_C_OFFSET_X1,			// wire 53
  CH_C_OFFSET_X1,			// wire 54
  CH_C_OFFSET_X1,			// wire 55
  CH_C_OFFSET_X1,			// wire 56
  CH_C_OFFSET_X1,			// wire 57
  CH_C_OFFSET_X1,			// wire 58
  CH_C_OFFSET_X1,			// wire 59
  CH_C_OFFSET_X1,			// wire 60
  CH_C_OFFSET_X1,			// wire 61
  CH_C_OFFSET_X1,			// wire 62
  CH_C_OFFSET_X1,			// wire 63
  CH_C_OFFSET_X1,			// wire 64
  CH_C_OFFSET_X1,			// wire 65
  CH_C_OFFSET_X1,			// wire 66
  CH_C_OFFSET_X1,			// wire 67
  CH_C_OFFSET_X1,			// wire 68
  CH_C_OFFSET_X1,			// wire 69
  CH_C_OFFSET_X1,			// wire 70
  CH_C_OFFSET_X1,			// wire 71
  CH_C_OFFSET_X1,			// wire 72
  CH_C_OFFSET_X1,			// wire 73
  CH_C_OFFSET_X1,			// wire 74
  CH_C_OFFSET_X1,			// wire 75
  CH_C_OFFSET_X1,			// wire 76
  CH_C_OFFSET_X1,			// wire 77
  CH_C_OFFSET_X1,			// wire 78
  CH_C_OFFSET_X1,			// wire 79
  CH_C_OFFSET_X1,			// wire 80
  CH_C_OFFSET_X1,			// wire 81
  CH_C_OFFSET_X1,			// wire 82
  CH_C_OFFSET_X1,			// wire 83
  CH_C_OFFSET_X1,			// wire 84
  CH_C_OFFSET_X1,			// wire 85
  CH_C_OFFSET_X1,			// wire 86
  CH_C_OFFSET_X1,			// wire 87
  CH_C_OFFSET_X1,			// wire 88
  CH_C_OFFSET_X1,			// wire 89
  CH_C_OFFSET_X1,			// wire 90
  CH_C_OFFSET_X1,			// wire 91
  CH_C_OFFSET_X1,			// wire 92
  CH_C_OFFSET_X1,			// wire 93
  CH_C_OFFSET_X1,			// wire 94
  CH_C_OFFSET_X1,			// wire 95
  CH_C_OFFSET_X1,			// wire 96
  CH_C_OFFSET_X1,			// wire 97
  CH_C_OFFSET_X1,			// wire 98
  CH_C_OFFSET_X1,			// wire 99
  CH_C_OFFSET_X1,			// wire 100
  CH_C_OFFSET_X1,			// wire 101
  CH_C_OFFSET_X1,			// wire 102
  CH_C_OFFSET_X1,			// wire 103
  CH_C_OFFSET_X1,			// wire 104
  CH_C_OFFSET_X1,			// wire 105
  CH_C_OFFSET_X1,			// wire 106
  CH_C_OFFSET_X1,			// wire 107
  CH_C_OFFSET_X1,			// wire 108
  CH_C_OFFSET_X1,			// wire 109
  CH_C_OFFSET_X1,			// wire 110
  CH_C_OFFSET_X1,			// wire 111
  CH_C_OFFSET_X1,			// wire 112
  CH_C_OFFSET_X1,			// wire 113
  CH_C_OFFSET_X1,			// wire 114
  CH_C_OFFSET_X1,			// wire 115
  CH_C_OFFSET_X1,			// wire 116
  CH_C_OFFSET_X1,			// wire 117
  CH_C_OFFSET_X1,			// wire 118
  CH_C_OFFSET_X1,			// wire 119
  CH_C_OFFSET_X1,			// wire 120
  CH_C_OFFSET_X1,			// wire 121
  CH_C_OFFSET_X1,			// wire 122
  CH_C_OFFSET_X1,			// wire 123
  CH_C_OFFSET_X1,			// wire 124
  CH_C_OFFSET_X1,			// wire 125
  CH_C_OFFSET_X1,			// wire 126
  CH_C_OFFSET_X1,			// wire 127
  CH_C_OFFSET_X1,			// wire 128
  CH_C_OFFSET_X1,			// wire 129
  CH_C_OFFSET_X1,			// wire 130
  CH_C_OFFSET_X1,			// wire 131
  CH_C_OFFSET_X1,			// wire 132
  CH_C_OFFSET_X1,			// wire 133
  CH_C_OFFSET_X1,			// wire 134
  CH_C_OFFSET_X1,			// wire 135
  CH_C_OFFSET_X1,			// wire 136
  CH_C_OFFSET_X1,			// wire 137
  CH_C_OFFSET_X1,			// wire 138
  CH_C_OFFSET_X1,			// wire 139
  CH_C_OFFSET_X1,			// wire 140
  CH_C_OFFSET_X1,			// wire 141
  CH_C_OFFSET_X1,			// wire 142
  CH_C_OFFSET_X1,			// wire 143
  CH_C_OFFSET_X1,			// wire 144
  CH_C_OFFSET_X1,			// wire 145
  CH_C_OFFSET_X1,			// wire 146
  CH_C_OFFSET_X1,			// wire 147
  CH_C_OFFSET_X1,			// wire 148
  CH_C_OFFSET_X1,			// wire 149
  CH_C_OFFSET_X1,			// wire 150
  CH_C_OFFSET_X1,			// wire 151
  CH_C_OFFSET_X1,			// wire 152
  CH_C_OFFSET_X1,			// wire 153
  CH_C_OFFSET_X1,			// wire 154
  CH_C_OFFSET_X1,			// wire 155
  CH_C_OFFSET_X1,			// wire 156
  CH_C_OFFSET_X1,			// wire 157
  CH_C_OFFSET_X1,			// wire 158
  CH_C_OFFSET_X1,			// wire 159
  CH_C_OFFSET_X1,			// wire 160
  CH_C_OFFSET_X1,			// wire 161
  CH_C_OFFSET_X1,			// wire 162
  CH_C_OFFSET_X1,			// wire 163
  CH_C_OFFSET_X1,			// wire 164
  CH_C_OFFSET_X1,			// wire 165
  CH_C_OFFSET_X1,			// wire 166
  CH_C_OFFSET_X1,			// wire 167
  CH_C_OFFSET_X1,			// wire 168
  CH_C_OFFSET_X1,			// wire 169
  CH_C_OFFSET_X1,			// wire 170
  CH_C_OFFSET_X1,			// wire 171
  CH_C_OFFSET_X1,			// wire 172
  CH_C_OFFSET_X1,			// wire 173
  CH_C_OFFSET_X1,			// wire 174
  CH_C_OFFSET_X1,			// wire 175
  CH_C_OFFSET_X1,			// wire 176
  CH_C_OFFSET_X1,			// wire 177
  CH_C_OFFSET_X1,			// wire 178
  CH_C_OFFSET_X1,			// wire 179
  CH_C_OFFSET_X1,			// wire 180
  CH_C_OFFSET_X1,			// wire 181
  CH_C_OFFSET_X1,			// wire 182
  CH_C_OFFSET_X1,			// wire 183
  CH_C_OFFSET_X1,			// wire 184
  CH_C_OFFSET_X1,			// wire 185
  CH_C_OFFSET_X1,			// wire 186
  CH_C_OFFSET_X1,			// wire 187
  CH_C_OFFSET_X1,			// wire 188
  CH_C_OFFSET_X1,			// wire 189
  CH_C_OFFSET_X1,			// wire 190
  CH_C_OFFSET_X1,			// wire 191
  CH_C_OFFSET_X1,			// wire 192
  CH_C_OFFSET_X1,			// wire 193
  CH_C_OFFSET_X1,			// wire 194
  CH_C_OFFSET_X1,			// wire 195
  CH_C_OFFSET_X1,			// wire 196
  CH_C_OFFSET_X1,			// wire 197
  CH_C_OFFSET_X1,			// wire 198
  CH_C_OFFSET_X1,			// wire 199
  CH_C_OFFSET_X1,			// wire 200
  CH_C_OFFSET_X1,			// wire 201
  CH_C_OFFSET_X1,			// wire 202
  CH_C_OFFSET_X1,			// wire 203
  CH_C_OFFSET_X1,			// wire 204
  CH_C_OFFSET_X1,			// wire 205
  CH_C_OFFSET_X1,			// wire 206
  CH_C_OFFSET_X1,			// wire 207
  CH_C_OFFSET_X1,			// wire 208
  CH_C_OFFSET_X1,			// wire 209
  CH_C_OFFSET_X1,			// wire 210
  CH_C_OFFSET_X1,			// wire 211
  CH_C_OFFSET_X1,			// wire 212
  CH_C_OFFSET_X1,			// wire 213
  CH_C_OFFSET_X1,			// wire 214
  CH_C_OFFSET_X1,			// wire 215
  CH_C_OFFSET_X1,			// wire 216
  CH_C_OFFSET_X1,			// wire 217
  CH_C_OFFSET_X1,			// wire 218
  CH_C_OFFSET_X1,			// wire 219
  CH_C_OFFSET_X1,			// wire 220
  CH_C_OFFSET_X1,			// wire 221
  CH_C_OFFSET_X1,			// wire 222
  CH_C_OFFSET_X1,			// wire 223
  CH_C_OFFSET_X1,			// wire 224
  CH_C_OFFSET_X1,			// wire 225
  CH_C_OFFSET_X1,			// wire 226
  CH_C_OFFSET_X1,			// wire 227
  CH_C_OFFSET_X1,			// wire 228
  CH_C_OFFSET_X1,			// wire 229
  CH_C_OFFSET_X1,			// wire 230
  CH_C_OFFSET_X1,			// wire 231
  CH_C_OFFSET_X1,			// wire 232
  CH_C_OFFSET_X1,			// wire 233
  CH_C_OFFSET_X1,			// wire 234
  CH_C_OFFSET_X1,			// wire 235
  CH_C_OFFSET_X1,			// wire 236
  CH_C_OFFSET_X1,			// wire 237
  CH_C_OFFSET_X1,			// wire 238
  CH_C_OFFSET_X1,			// wire 239
  CH_C_OFFSET_X1,			// wire 240
  CH_C_OFFSET_X1,			// wire 241
  CH_C_OFFSET_X1,			// wire 242
  CH_C_OFFSET_X1,			// wire 243
  CH_C_OFFSET_X1,			// wire 244
  CH_C_OFFSET_X1,			// wire 245
  CH_C_OFFSET_X1,			// wire 246
  CH_C_OFFSET_X1,			// wire 247
  CH_C_OFFSET_X1,			// wire 248
  CH_C_OFFSET_X1,			// wire 249
  CH_C_OFFSET_X1,			// wire 250
  CH_C_OFFSET_X1,			// wire 251
  CH_C_OFFSET_X1,			// wire 252
  CH_C_OFFSET_X1,			// wire 253
  CH_C_OFFSET_X1,			// wire 254
  CH_C_OFFSET_X1,			// wire 255
  CH_C_OFFSET_X1,			// wire 256
  CH_C_OFFSET_X1,			// wire 257
  CH_C_OFFSET_X1,			// wire 258
  CH_C_OFFSET_X1,			// wire 259
  CH_C_OFFSET_X1,			// wire 260
  CH_C_OFFSET_X1,			// wire 261
  CH_C_OFFSET_X1,			// wire 262
  CH_C_OFFSET_X1,			// wire 263
  CH_C_OFFSET_X1,			// wire 264
  CH_C_OFFSET_X1,			// wire 265
  CH_C_OFFSET_X1,			// wire 266
  CH_C_OFFSET_X1,			// wire 267
  CH_C_OFFSET_X1,			// wire 268
  CH_C_OFFSET_X1,			// wire 269
  CH_C_OFFSET_X1,			// wire 270
  CH_C_OFFSET_X1,			// wire 271
  CH_C_OFFSET_X1,			// wire 272
  CH_C_OFFSET_X1,			// wire 273
  CH_C_OFFSET_X1,			// wire 274
  CH_C_OFFSET_X1,			// wire 275
  CH_C_OFFSET_X1,			// wire 276
  CH_C_OFFSET_X1,			// wire 277
  CH_C_OFFSET_X1,			// wire 278
  CH_C_OFFSET_X1,			// wire 279
  CH_C_OFFSET_X1,			// wire 280
  CH_C_OFFSET_X1,			// wire 281
  CH_C_OFFSET_X1,			// wire 282
  CH_C_OFFSET_X1,			// wire 283
  CH_C_OFFSET_X1,			// wire 284
  CH_C_OFFSET_X1,			// wire 285
  CH_C_OFFSET_X1,			// wire 286
  CH_C_OFFSET_X1,			// wire 287
  CH_C_OFFSET_X1,			// wire 288
  CH_C_OFFSET_X1,			// wire 289
  CH_C_OFFSET_X1,			// wire 290
  CH_C_OFFSET_X1,			// wire 291
  CH_C_OFFSET_X1,			// wire 292
  CH_C_OFFSET_X1,			// wire 293
  CH_C_OFFSET_X1,			// wire 294
  CH_C_OFFSET_X1,			// wire 295
  CH_C_OFFSET_X1,			// wire 296
  CH_C_OFFSET_X1,			// wire 297
  CH_C_OFFSET_X1,			// wire 298
  CH_C_OFFSET_X1,			// wire 299
  CH_C_OFFSET_X1,			// wire 300
  CH_C_OFFSET_X1,			// wire 301
  CH_C_OFFSET_X1,			// wire 302
  CH_C_OFFSET_X1,			// wire 303
  CH_C_OFFSET_X1,			// wire 304
  CH_C_OFFSET_X1,			// wire 305
  CH_C_OFFSET_X1,			// wire 306
  CH_C_OFFSET_X1,			// wire 307
  CH_C_OFFSET_X1,			// wire 308
  CH_C_OFFSET_X1,			// wire 309
  CH_C_OFFSET_X1,			// wire 310
  CH_C_OFFSET_X1,			// wire 311
  CH_C_OFFSET_X1,			// wire 312
  CH_C_OFFSET_X1,			// wire 313
  CH_C_OFFSET_X1,			// wire 314
  CH_C_OFFSET_X1,			// wire 315
  CH_C_OFFSET_X1,			// wire 316
  CH_C_OFFSET_X1,			// wire 317
  CH_C_OFFSET_X1,			// wire 318
  CH_C_OFFSET_X1,			// wire 319
  CH_C_OFFSET_X1,			// wire 320
  CH_C_OFFSET_X1,			// wire 321
  CH_C_OFFSET_X1,			// wire 322
  CH_C_OFFSET_X1,			// wire 323
  CH_C_OFFSET_X1,			// wire 324
  CH_C_OFFSET_X1,			// wire 325
  CH_C_OFFSET_X1,			// wire 326
  CH_C_OFFSET_X1,			// wire 327
  CH_C_OFFSET_X1,			// wire 328
  CH_C_OFFSET_X1,			// wire 329
  CH_C_OFFSET_X1,			// wire 330
  CH_C_OFFSET_X1,			// wire 331
  CH_C_OFFSET_X1,			// wire 332
  CH_C_OFFSET_X1,			// wire 333
  CH_C_OFFSET_X1,			// wire 334
  CH_C_OFFSET_X1,			// wire 335
  CH_C_OFFSET_X1,			// wire 336
  CH_C_OFFSET_X1,			// wire 337
  CH_C_OFFSET_X1,			// wire 338
  CH_C_OFFSET_X1,			// wire 339
  CH_C_OFFSET_X1,			// wire 340
  CH_C_OFFSET_X1,			// wire 341
  CH_C_OFFSET_X1,			// wire 342
  CH_C_OFFSET_X1,			// wire 343
  CH_C_OFFSET_X1,			// wire 344
  CH_C_OFFSET_X1,			// wire 345
  CH_C_OFFSET_X1,			// wire 346
  CH_C_OFFSET_X1,			// wire 347
  CH_C_OFFSET_X1,			// wire 348
  CH_C_OFFSET_X1,			// wire 349
  CH_C_OFFSET_X1,			// wire 350
  CH_C_OFFSET_X1,			// wire 351
  CH_C_OFFSET_X1,			// wire 352
  CH_C_OFFSET_X1,			// wire 353
  CH_C_OFFSET_X1,			// wire 354
  CH_C_OFFSET_X1,			// wire 355
  CH_C_OFFSET_X1,			// wire 356
  CH_C_OFFSET_X1,			// wire 357
  CH_C_OFFSET_X1,			// wire 358
  CH_C_OFFSET_X1,			// wire 359
  CH_C_OFFSET_X1,			// wire 360
  CH_C_OFFSET_X1,			// wire 361
  CH_C_OFFSET_X1,			// wire 362
  CH_C_OFFSET_X1,			// wire 363
  CH_C_OFFSET_X1,			// wire 364
  CH_C_OFFSET_X1,			// wire 365
  CH_C_OFFSET_X1,			// wire 366
  CH_C_OFFSET_X1,			// wire 367
  CH_C_OFFSET_X1,			// wire 368
  CH_C_OFFSET_X1,			// wire 369
  CH_C_OFFSET_X1,			// wire 370
  CH_C_OFFSET_X1,			// wire 371
  CH_C_OFFSET_X1,			// wire 372
  CH_C_OFFSET_X1,			// wire 373
  CH_C_OFFSET_X1,			// wire 374
  CH_C_OFFSET_X1,			// wire 375
  CH_C_OFFSET_X1,			// wire 376
  CH_C_OFFSET_X1,			// wire 377
  CH_C_OFFSET_X1,			// wire 378
  CH_C_OFFSET_X1,			// wire 379
  CH_C_OFFSET_X1,			// wire 380
  CH_C_OFFSET_X1,			// wire 381
  CH_C_OFFSET_X1,			// wire 382
  CH_C_OFFSET_X1,			// wire 383
  CH_C_OFFSET_X1,			// wire 384
  CH_C_OFFSET_X1,			// wire 385
  CH_C_OFFSET_X1,			// wire 386
  CH_C_OFFSET_X1,			// wire 387
  CH_C_OFFSET_X1,			// wire 388
  CH_C_OFFSET_X1,			// wire 389
  CH_C_OFFSET_X1,			// wire 390
  CH_C_OFFSET_X1,			// wire 391
  CH_C_OFFSET_X1,			// wire 392
  CH_C_OFFSET_X1,			// wire 393
  CH_C_OFFSET_X1,			// wire 394
  CH_C_OFFSET_X1,			// wire 395
  CH_C_OFFSET_X1,			// wire 396
  CH_C_OFFSET_X1,			// wire 397
  CH_C_OFFSET_X1,			// wire 398
  CH_C_OFFSET_X1,			// wire 399
  CH_C_OFFSET_X1,			// wire 400
  CH_C_OFFSET_X1,			// wire 401
  CH_C_OFFSET_X1,			// wire 402
  CH_C_OFFSET_X1,			// wire 403
  CH_C_OFFSET_X1,			// wire 404
  CH_C_OFFSET_X1,			// wire 405
  CH_C_OFFSET_X1,			// wire 406
  CH_C_OFFSET_X1,			// wire 407
  CH_C_OFFSET_X1,			// wire 408
  CH_C_OFFSET_X1,			// wire 409
  CH_C_OFFSET_X1,			// wire 410
  CH_C_OFFSET_X1,			// wire 411
  CH_C_OFFSET_X1,			// wire 412
  CH_C_OFFSET_X1,			// wire 413
  CH_C_OFFSET_X1,			// wire 414
  CH_C_OFFSET_X1,			// wire 415
  CH_C_OFFSET_X1,			// wire 416
  CH_C_OFFSET_X1,			// wire 417
  CH_C_OFFSET_X1,			// wire 418
  CH_C_OFFSET_X1,			// wire 419
  CH_C_OFFSET_X1,			// wire 420
  CH_C_OFFSET_X1,			// wire 421
  CH_C_OFFSET_X1,			// wire 422
  CH_C_OFFSET_X1,			// wire 423
  CH_C_OFFSET_X1,			// wire 424
  CH_C_OFFSET_X1,			// wire 425
  CH_C_OFFSET_X1,			// wire 426
  CH_C_OFFSET_X1,			// wire 427
  CH_C_OFFSET_X1,			// wire 428
  CH_C_OFFSET_X1,			// wire 429
  CH_C_OFFSET_X1,			// wire 430
  CH_C_OFFSET_X1,			// wire 431
  CH_C_OFFSET_X1,			// wire 432
  CH_C_OFFSET_X1,			// wire 433
  CH_C_OFFSET_X1,			// wire 434
  CH_C_OFFSET_X1,			// wire 435
  CH_C_OFFSET_X1,			// wire 436
  CH_C_OFFSET_X1,			// wire 437
  CH_C_OFFSET_X1,			// wire 438
  CH_C_OFFSET_X1,			// wire 439
  CH_C_OFFSET_X1,			// wire 440
  CH_C_OFFSET_X1,			// wire 441
  CH_C_OFFSET_X1,			// wire 442
  CH_C_OFFSET_X1,			// wire 443
  CH_C_OFFSET_X1,			// wire 444
  CH_C_OFFSET_X1,			// wire 445
  CH_C_OFFSET_X1,			// wire 446
  CH_C_OFFSET_X1,			// wire 447
  CH_C_OFFSET_X1			// wire 448
};

static short
offset_cs1[CH_C_MAX_S1] = {
  CH_C_OFFSET_S1,			// wire 1
  CH_C_OFFSET_S1,			// wire 2
  CH_C_OFFSET_S1,			// wire 3
  CH_C_OFFSET_S1,			// wire 4
  CH_C_OFFSET_S1,			// wire 5
  CH_C_OFFSET_S1,			// wire 6
  CH_C_OFFSET_S1,			// wire 7
  CH_C_OFFSET_S1,			// wire 8
  CH_C_OFFSET_S1,			// wire 9
  CH_C_OFFSET_S1,			// wire 10
  CH_C_OFFSET_S1,			// wire 11
  CH_C_OFFSET_S1,			// wire 12
  CH_C_OFFSET_S1,			// wire 13
  CH_C_OFFSET_S1,			// wire 14
  CH_C_OFFSET_S1,			// wire 15
  CH_C_OFFSET_S1,			// wire 16
  CH_C_OFFSET_S1,			// wire 17
  CH_C_OFFSET_S1,			// wire 18
  CH_C_OFFSET_S1,			// wire 19
  CH_C_OFFSET_S1,			// wire 20
  CH_C_OFFSET_S1,			// wire 21
  CH_C_OFFSET_S1,			// wire 22
  CH_C_OFFSET_S1,			// wire 23
  CH_C_OFFSET_S1,			// wire 24
  CH_C_OFFSET_S1,			// wire 25
  CH_C_OFFSET_S1,			// wire 26
  CH_C_OFFSET_S1,			// wire 27
  CH_C_OFFSET_S1,			// wire 28
  CH_C_OFFSET_S1,			// wire 29
  CH_C_OFFSET_S1,			// wire 30
  CH_C_OFFSET_S1,			// wire 31
  CH_C_OFFSET_S1,			// wire 32
  CH_C_OFFSET_S1,			// wire 33
  CH_C_OFFSET_S1,			// wire 34
  CH_C_OFFSET_S1,			// wire 35
  CH_C_OFFSET_S1,			// wire 36
  CH_C_OFFSET_S1,			// wire 37
  CH_C_OFFSET_S1,			// wire 38
  CH_C_OFFSET_S1,			// wire 39
  CH_C_OFFSET_S1,			// wire 40
  CH_C_OFFSET_S1,			// wire 41
  CH_C_OFFSET_S1,			// wire 42
  CH_C_OFFSET_S1,			// wire 43
  CH_C_OFFSET_S1,			// wire 44
  CH_C_OFFSET_S1,			// wire 45
  CH_C_OFFSET_S1,			// wire 46
  CH_C_OFFSET_S1,			// wire 47
  CH_C_OFFSET_S1,			// wire 48
  CH_C_OFFSET_S1,			// wire 49
  CH_C_OFFSET_S1,			// wire 50
  CH_C_OFFSET_S1,			// wire 51
  CH_C_OFFSET_S1,			// wire 52
  CH_C_OFFSET_S1,			// wire 53
  CH_C_OFFSET_S1,			// wire 54
  CH_C_OFFSET_S1,			// wire 55
  CH_C_OFFSET_S1,			// wire 56
  CH_C_OFFSET_S1,			// wire 57
  CH_C_OFFSET_S1,			// wire 58
  CH_C_OFFSET_S1,			// wire 59
  CH_C_OFFSET_S1,			// wire 60
  CH_C_OFFSET_S1,			// wire 61
  CH_C_OFFSET_S1,			// wire 62
  CH_C_OFFSET_S1,			// wire 63
  CH_C_OFFSET_S1,			// wire 64
  CH_C_OFFSET_S1,			// wire 65
  CH_C_OFFSET_S1,			// wire 66
  CH_C_OFFSET_S1,			// wire 67
  CH_C_OFFSET_S1,			// wire 68
  CH_C_OFFSET_S1,			// wire 69
  CH_C_OFFSET_S1,			// wire 70
  CH_C_OFFSET_S1,			// wire 71
  CH_C_OFFSET_S1,			// wire 72
  CH_C_OFFSET_S1,			// wire 73
  CH_C_OFFSET_S1,			// wire 74
  CH_C_OFFSET_S1,			// wire 75
  CH_C_OFFSET_S1,			// wire 76
  CH_C_OFFSET_S1,			// wire 77
  CH_C_OFFSET_S1,			// wire 78
  CH_C_OFFSET_S1,			// wire 79
  CH_C_OFFSET_S1,			// wire 80
  CH_C_OFFSET_S1,			// wire 81
  CH_C_OFFSET_S1,			// wire 82
  CH_C_OFFSET_S1,			// wire 83
  CH_C_OFFSET_S1,			// wire 84
  CH_C_OFFSET_S1,			// wire 85
  CH_C_OFFSET_S1,			// wire 86
  CH_C_OFFSET_S1,			// wire 87
  CH_C_OFFSET_S1,			// wire 88
  CH_C_OFFSET_S1,			// wire 89
  CH_C_OFFSET_S1,			// wire 90
  CH_C_OFFSET_S1,			// wire 91
  CH_C_OFFSET_S1,			// wire 92
  CH_C_OFFSET_S1,			// wire 93
  CH_C_OFFSET_S1,			// wire 94
  CH_C_OFFSET_S1,			// wire 95
  CH_C_OFFSET_S1,			// wire 96
  CH_C_OFFSET_S1,			// wire 97
  CH_C_OFFSET_S1,			// wire 98
  CH_C_OFFSET_S1,			// wire 99
  CH_C_OFFSET_S1,			// wire 100
  CH_C_OFFSET_S1,			// wire 101
  CH_C_OFFSET_S1,			// wire 102
  CH_C_OFFSET_S1,			// wire 103
  CH_C_OFFSET_S1,			// wire 104
  CH_C_OFFSET_S1,			// wire 105
  CH_C_OFFSET_S1,			// wire 106
  CH_C_OFFSET_S1,			// wire 107
  CH_C_OFFSET_S1,			// wire 108
  CH_C_OFFSET_S1,			// wire 109
  CH_C_OFFSET_S1,			// wire 110
  CH_C_OFFSET_S1,			// wire 111
  CH_C_OFFSET_S1,			// wire 112
  CH_C_OFFSET_S1,			// wire 113
  CH_C_OFFSET_S1,			// wire 114
  CH_C_OFFSET_S1,			// wire 115
  CH_C_OFFSET_S1,			// wire 116
  CH_C_OFFSET_S1,			// wire 117
  CH_C_OFFSET_S1,			// wire 118
  CH_C_OFFSET_S1,			// wire 119
  CH_C_OFFSET_S1,			// wire 120
  CH_C_OFFSET_S1,			// wire 121
  CH_C_OFFSET_S1,			// wire 122
  CH_C_OFFSET_S1,			// wire 123
  CH_C_OFFSET_S1,			// wire 124
  CH_C_OFFSET_S1,			// wire 125
  CH_C_OFFSET_S1,			// wire 126
  CH_C_OFFSET_S1,			// wire 127
  CH_C_OFFSET_S1,			// wire 128
  CH_C_OFFSET_S1,			// wire 129
  CH_C_OFFSET_S1,			// wire 130
  CH_C_OFFSET_S1,			// wire 131
  CH_C_OFFSET_S1,			// wire 132
  CH_C_OFFSET_S1,			// wire 133
  CH_C_OFFSET_S1,			// wire 134
  CH_C_OFFSET_S1,			// wire 135
  CH_C_OFFSET_S1,			// wire 136
  CH_C_OFFSET_S1,			// wire 137
  CH_C_OFFSET_S1,			// wire 138
  CH_C_OFFSET_S1,			// wire 139
  CH_C_OFFSET_S1,			// wire 140
  CH_C_OFFSET_S1,			// wire 141
  CH_C_OFFSET_S1,			// wire 142
  CH_C_OFFSET_S1,			// wire 143
  CH_C_OFFSET_S1,			// wire 144
  CH_C_OFFSET_S1,			// wire 145
  CH_C_OFFSET_S1,			// wire 146
  CH_C_OFFSET_S1,			// wire 147
  CH_C_OFFSET_S1,			// wire 148
  CH_C_OFFSET_S1,			// wire 149
  CH_C_OFFSET_S1,			// wire 150
  CH_C_OFFSET_S1,			// wire 151
  CH_C_OFFSET_S1,			// wire 152
  CH_C_OFFSET_S1,			// wire 153
  CH_C_OFFSET_S1,			// wire 154
  CH_C_OFFSET_S1,			// wire 155
  CH_C_OFFSET_S1,			// wire 156
  CH_C_OFFSET_S1,			// wire 157
  CH_C_OFFSET_S1,			// wire 158
  CH_C_OFFSET_S1,			// wire 159
  CH_C_OFFSET_S1,			// wire 160
  CH_C_OFFSET_S1,			// wire 161
  CH_C_OFFSET_S1,			// wire 162
  CH_C_OFFSET_S1,			// wire 163
  CH_C_OFFSET_S1,			// wire 164
  CH_C_OFFSET_S1,			// wire 165
  CH_C_OFFSET_S1,			// wire 166
  CH_C_OFFSET_S1,			// wire 167
  CH_C_OFFSET_S1,			// wire 168
  CH_C_OFFSET_S1,			// wire 169
  CH_C_OFFSET_S1,			// wire 170
  CH_C_OFFSET_S1,			// wire 171
  CH_C_OFFSET_S1,			// wire 172
  CH_C_OFFSET_S1,			// wire 173
  CH_C_OFFSET_S1,			// wire 174
  CH_C_OFFSET_S1,			// wire 175
  CH_C_OFFSET_S1,			// wire 176
  CH_C_OFFSET_S1,			// wire 177
  CH_C_OFFSET_S1,			// wire 178
  CH_C_OFFSET_S1,			// wire 179
  CH_C_OFFSET_S1,			// wire 180
  CH_C_OFFSET_S1,			// wire 181
  CH_C_OFFSET_S1,			// wire 182
  CH_C_OFFSET_S1,			// wire 183
  CH_C_OFFSET_S1,			// wire 184
  CH_C_OFFSET_S1,			// wire 185
  CH_C_OFFSET_S1,			// wire 186
  CH_C_OFFSET_S1,			// wire 187
  CH_C_OFFSET_S1,			// wire 188
  CH_C_OFFSET_S1,			// wire 189
  CH_C_OFFSET_S1,			// wire 190
  CH_C_OFFSET_S1,			// wire 191
  CH_C_OFFSET_S1,			// wire 192
  CH_C_OFFSET_S1,			// wire 193
  CH_C_OFFSET_S1,			// wire 194
  CH_C_OFFSET_S1,			// wire 195
  CH_C_OFFSET_S1,			// wire 196
  CH_C_OFFSET_S1,			// wire 197
  CH_C_OFFSET_S1,			// wire 198
  CH_C_OFFSET_S1,			// wire 199
  CH_C_OFFSET_S1,			// wire 200
  CH_C_OFFSET_S1,			// wire 201
  CH_C_OFFSET_S1,			// wire 202
  CH_C_OFFSET_S1,			// wire 203
  CH_C_OFFSET_S1,			// wire 204
  CH_C_OFFSET_S1,			// wire 205
  CH_C_OFFSET_S1,			// wire 206
  CH_C_OFFSET_S1,			// wire 207
  CH_C_OFFSET_S1,			// wire 208
  CH_C_OFFSET_S1,			// wire 209
  CH_C_OFFSET_S1,			// wire 210
  CH_C_OFFSET_S1,			// wire 211
  CH_C_OFFSET_S1,			// wire 212
  CH_C_OFFSET_S1,			// wire 213
  CH_C_OFFSET_S1,			// wire 214
  CH_C_OFFSET_S1,			// wire 215
  CH_C_OFFSET_S1,			// wire 216
  CH_C_OFFSET_S1,			// wire 217
  CH_C_OFFSET_S1,			// wire 218
  CH_C_OFFSET_S1,			// wire 219
  CH_C_OFFSET_S1,			// wire 220
  CH_C_OFFSET_S1,			// wire 221
  CH_C_OFFSET_S1,			// wire 222
  CH_C_OFFSET_S1,			// wire 223
  CH_C_OFFSET_S1,			// wire 224
  CH_C_OFFSET_S1,			// wire 225
  CH_C_OFFSET_S1,			// wire 226
  CH_C_OFFSET_S1,			// wire 227
  CH_C_OFFSET_S1,			// wire 228
  CH_C_OFFSET_S1,			// wire 229
  CH_C_OFFSET_S1,			// wire 230
  CH_C_OFFSET_S1,			// wire 231
  CH_C_OFFSET_S1,			// wire 232
  CH_C_OFFSET_S1,			// wire 233
  CH_C_OFFSET_S1,			// wire 234
  CH_C_OFFSET_S1,			// wire 235
  CH_C_OFFSET_S1,			// wire 236
  CH_C_OFFSET_S1,			// wire 237
  CH_C_OFFSET_S1,			// wire 238
  CH_C_OFFSET_S1,			// wire 239
  CH_C_OFFSET_S1,			// wire 240
  CH_C_OFFSET_S1,			// wire 241
  CH_C_OFFSET_S1,			// wire 242
  CH_C_OFFSET_S1,			// wire 243
  CH_C_OFFSET_S1,			// wire 244
  CH_C_OFFSET_S1,			// wire 245
  CH_C_OFFSET_S1,			// wire 246
  CH_C_OFFSET_S1,			// wire 247
  CH_C_OFFSET_S1,			// wire 248
  CH_C_OFFSET_S1,			// wire 249
  CH_C_OFFSET_S1,			// wire 250
  CH_C_OFFSET_S1,			// wire 251
  CH_C_OFFSET_S1,			// wire 252
  CH_C_OFFSET_S1,			// wire 253
  CH_C_OFFSET_S1,			// wire 254
  CH_C_OFFSET_S1,			// wire 255
  CH_C_OFFSET_S1,			// wire 256
  CH_C_OFFSET_S1,			// wire 257
  CH_C_OFFSET_S1,			// wire 258
  CH_C_OFFSET_S1,			// wire 259
  CH_C_OFFSET_S1,			// wire 260
  CH_C_OFFSET_S1,			// wire 261
  CH_C_OFFSET_S1,			// wire 262
  CH_C_OFFSET_S1,			// wire 263
  CH_C_OFFSET_S1,			// wire 264
  CH_C_OFFSET_S1,			// wire 265
  CH_C_OFFSET_S1,			// wire 266
  CH_C_OFFSET_S1,			// wire 267
  CH_C_OFFSET_S1,			// wire 268
  CH_C_OFFSET_S1,			// wire 269
  CH_C_OFFSET_S1,			// wire 270
  CH_C_OFFSET_S1,			// wire 271
  CH_C_OFFSET_S1,			// wire 272
  CH_C_OFFSET_S1,			// wire 273
  CH_C_OFFSET_S1,			// wire 274
  CH_C_OFFSET_S1,			// wire 275
  CH_C_OFFSET_S1,			// wire 276
  CH_C_OFFSET_S1,			// wire 277
  CH_C_OFFSET_S1,			// wire 278
  CH_C_OFFSET_S1,			// wire 279
  CH_C_OFFSET_S1,			// wire 280
  CH_C_OFFSET_S1,			// wire 281
  CH_C_OFFSET_S1,			// wire 282
  CH_C_OFFSET_S1,			// wire 283
  CH_C_OFFSET_S1,			// wire 284
  CH_C_OFFSET_S1,			// wire 285
  CH_C_OFFSET_S1,			// wire 286
  CH_C_OFFSET_S1,			// wire 287
  CH_C_OFFSET_S1,			// wire 288
  CH_C_OFFSET_S1,			// wire 289
  CH_C_OFFSET_S1,			// wire 290
  CH_C_OFFSET_S1,			// wire 291
  CH_C_OFFSET_S1,			// wire 292
  CH_C_OFFSET_S1,			// wire 293
  CH_C_OFFSET_S1,			// wire 294
  CH_C_OFFSET_S1,			// wire 295
  CH_C_OFFSET_S1,			// wire 296
  CH_C_OFFSET_S1,			// wire 297
  CH_C_OFFSET_S1,			// wire 298
  CH_C_OFFSET_S1,			// wire 299
  CH_C_OFFSET_S1,			// wire 300
  CH_C_OFFSET_S1,			// wire 301
  CH_C_OFFSET_S1,			// wire 302
  CH_C_OFFSET_S1,			// wire 303
  CH_C_OFFSET_S1,			// wire 304
  CH_C_OFFSET_S1,			// wire 305
  CH_C_OFFSET_S1,			// wire 306
  CH_C_OFFSET_S1,			// wire 307
  CH_C_OFFSET_S1,			// wire 308
  CH_C_OFFSET_S1,			// wire 309
  CH_C_OFFSET_S1,			// wire 310
  CH_C_OFFSET_S1,			// wire 311
  CH_C_OFFSET_S1,			// wire 312
  CH_C_OFFSET_S1,			// wire 313
  CH_C_OFFSET_S1,			// wire 314
  CH_C_OFFSET_S1,			// wire 315
  CH_C_OFFSET_S1,			// wire 316
  CH_C_OFFSET_S1,			// wire 317
  CH_C_OFFSET_S1,			// wire 318
  CH_C_OFFSET_S1,			// wire 319
  CH_C_OFFSET_S1,			// wire 320
  CH_C_OFFSET_S1,			// wire 321
  CH_C_OFFSET_S1,			// wire 322
  CH_C_OFFSET_S1,			// wire 323
  CH_C_OFFSET_S1,			// wire 324
  CH_C_OFFSET_S1,			// wire 325
  CH_C_OFFSET_S1,			// wire 326
  CH_C_OFFSET_S1,			// wire 327
  CH_C_OFFSET_S1,			// wire 328
  CH_C_OFFSET_S1,			// wire 329
  CH_C_OFFSET_S1,			// wire 330
  CH_C_OFFSET_S1,			// wire 331
  CH_C_OFFSET_S1,			// wire 332
  CH_C_OFFSET_S1,			// wire 333
  CH_C_OFFSET_S1,			// wire 334
  CH_C_OFFSET_S1,			// wire 335
  CH_C_OFFSET_S1,			// wire 336
  CH_C_OFFSET_S1,			// wire 337
  CH_C_OFFSET_S1,			// wire 338
  CH_C_OFFSET_S1,			// wire 339
  CH_C_OFFSET_S1,			// wire 340
  CH_C_OFFSET_S1,			// wire 341
  CH_C_OFFSET_S1,			// wire 342
  CH_C_OFFSET_S1,			// wire 343
  CH_C_OFFSET_S1,			// wire 344
  CH_C_OFFSET_S1,			// wire 345
  CH_C_OFFSET_S1,			// wire 346
  CH_C_OFFSET_S1,			// wire 347
  CH_C_OFFSET_S1,			// wire 348
  CH_C_OFFSET_S1,			// wire 349
  CH_C_OFFSET_S1,			// wire 350
  CH_C_OFFSET_S1,			// wire 351
  CH_C_OFFSET_S1			// wire 352
};

static short
offset_cx2[CH_C_MAX_X2] = {
  CH_C_OFFSET_X2,			// wire 1
  CH_C_OFFSET_X2,			// wire 2
  CH_C_OFFSET_X2,			// wire 3
  CH_C_OFFSET_X2,			// wire 4
  CH_C_OFFSET_X2,			// wire 5
  CH_C_OFFSET_X2,			// wire 6
  CH_C_OFFSET_X2,			// wire 7
  CH_C_OFFSET_X2,			// wire 8
  CH_C_OFFSET_X2,			// wire 9
  CH_C_OFFSET_X2,			// wire 10
  CH_C_OFFSET_X2,			// wire 11
  CH_C_OFFSET_X2,			// wire 12
  CH_C_OFFSET_X2,			// wire 13
  CH_C_OFFSET_X2,			// wire 14
  CH_C_OFFSET_X2,			// wire 15
  CH_C_OFFSET_X2,			// wire 16
  CH_C_OFFSET_X2,			// wire 17
  CH_C_OFFSET_X2,			// wire 18
  CH_C_OFFSET_X2,			// wire 19
  CH_C_OFFSET_X2,			// wire 20
  CH_C_OFFSET_X2,			// wire 21
  CH_C_OFFSET_X2,			// wire 22
  CH_C_OFFSET_X2,			// wire 23
  CH_C_OFFSET_X2,			// wire 24
  CH_C_OFFSET_X2,			// wire 25
  CH_C_OFFSET_X2,			// wire 26
  CH_C_OFFSET_X2,			// wire 27
  CH_C_OFFSET_X2,			// wire 28
  CH_C_OFFSET_X2,			// wire 29
  CH_C_OFFSET_X2,			// wire 30
  CH_C_OFFSET_X2,			// wire 31
  CH_C_OFFSET_X2,			// wire 32
  CH_C_OFFSET_X2,			// wire 33
  CH_C_OFFSET_X2,			// wire 34
  CH_C_OFFSET_X2,			// wire 35
  CH_C_OFFSET_X2,			// wire 36
  CH_C_OFFSET_X2,			// wire 37
  CH_C_OFFSET_X2,			// wire 38
  CH_C_OFFSET_X2,			// wire 39
  CH_C_OFFSET_X2,			// wire 40
  CH_C_OFFSET_X2,			// wire 41
  CH_C_OFFSET_X2,			// wire 42
  CH_C_OFFSET_X2,			// wire 43
  CH_C_OFFSET_X2,			// wire 44
  CH_C_OFFSET_X2,			// wire 45
  CH_C_OFFSET_X2,			// wire 46
  CH_C_OFFSET_X2,			// wire 47
  CH_C_OFFSET_X2,			// wire 48
  CH_C_OFFSET_X2,			// wire 49
  CH_C_OFFSET_X2,			// wire 50
  CH_C_OFFSET_X2,			// wire 51
  CH_C_OFFSET_X2,			// wire 52
  CH_C_OFFSET_X2,			// wire 53
  CH_C_OFFSET_X2,			// wire 54
  CH_C_OFFSET_X2,			// wire 55
  CH_C_OFFSET_X2,			// wire 56
  CH_C_OFFSET_X2,			// wire 57
  CH_C_OFFSET_X2,			// wire 58
  CH_C_OFFSET_X2,			// wire 59
  CH_C_OFFSET_X2,			// wire 60
  CH_C_OFFSET_X2,			// wire 61
  CH_C_OFFSET_X2,			// wire 62
  CH_C_OFFSET_X2,			// wire 63
  CH_C_OFFSET_X2,			// wire 64
  CH_C_OFFSET_X2,			// wire 65
  CH_C_OFFSET_X2,			// wire 66
  CH_C_OFFSET_X2,			// wire 67
  CH_C_OFFSET_X2,			// wire 68
  CH_C_OFFSET_X2,			// wire 69
  CH_C_OFFSET_X2,			// wire 70
  CH_C_OFFSET_X2,			// wire 71
  CH_C_OFFSET_X2,			// wire 72
  CH_C_OFFSET_X2,			// wire 73
  CH_C_OFFSET_X2,			// wire 74
  CH_C_OFFSET_X2,			// wire 75
  CH_C_OFFSET_X2,			// wire 76
  CH_C_OFFSET_X2,			// wire 77
  CH_C_OFFSET_X2,			// wire 78
  CH_C_OFFSET_X2,			// wire 79
  CH_C_OFFSET_X2,			// wire 80
  CH_C_OFFSET_X2,			// wire 81
  CH_C_OFFSET_X2,			// wire 82
  CH_C_OFFSET_X2,			// wire 83
  CH_C_OFFSET_X2,			// wire 84
  CH_C_OFFSET_X2,			// wire 85
  CH_C_OFFSET_X2,			// wire 86
  CH_C_OFFSET_X2,			// wire 87
  CH_C_OFFSET_X2,			// wire 88
  CH_C_OFFSET_X2,			// wire 89
  CH_C_OFFSET_X2,			// wire 90
  CH_C_OFFSET_X2,			// wire 91
  CH_C_OFFSET_X2,			// wire 92
  CH_C_OFFSET_X2,			// wire 93
  CH_C_OFFSET_X2,			// wire 94
  CH_C_OFFSET_X2,			// wire 95
  CH_C_OFFSET_X2,			// wire 96
  CH_C_OFFSET_X2,			// wire 97
  CH_C_OFFSET_X2,			// wire 98
  CH_C_OFFSET_X2,			// wire 99
  CH_C_OFFSET_X2,			// wire 100
  CH_C_OFFSET_X2,			// wire 101
  CH_C_OFFSET_X2,			// wire 102
  CH_C_OFFSET_X2,			// wire 103
  CH_C_OFFSET_X2,			// wire 104
  CH_C_OFFSET_X2,			// wire 105
  CH_C_OFFSET_X2,			// wire 106
  CH_C_OFFSET_X2,			// wire 107
  CH_C_OFFSET_X2,			// wire 108
  CH_C_OFFSET_X2,			// wire 109
  CH_C_OFFSET_X2,			// wire 110
  CH_C_OFFSET_X2,			// wire 111
  CH_C_OFFSET_X2,			// wire 112
  CH_C_OFFSET_X2,			// wire 113
  CH_C_OFFSET_X2,			// wire 114
  CH_C_OFFSET_X2,			// wire 115
  CH_C_OFFSET_X2,			// wire 116
  CH_C_OFFSET_X2,			// wire 117
  CH_C_OFFSET_X2,			// wire 118
  CH_C_OFFSET_X2,			// wire 119
  CH_C_OFFSET_X2,			// wire 120
  CH_C_OFFSET_X2,			// wire 121
  CH_C_OFFSET_X2,			// wire 122
  CH_C_OFFSET_X2,			// wire 123
  CH_C_OFFSET_X2,			// wire 124
  CH_C_OFFSET_X2,			// wire 125
  CH_C_OFFSET_X2,			// wire 126
  CH_C_OFFSET_X2,			// wire 127
  CH_C_OFFSET_X2,			// wire 128
  CH_C_OFFSET_X2,			// wire 129
  CH_C_OFFSET_X2,			// wire 130
  CH_C_OFFSET_X2,			// wire 131
  CH_C_OFFSET_X2,			// wire 132
  CH_C_OFFSET_X2,			// wire 133
  CH_C_OFFSET_X2,			// wire 134
  CH_C_OFFSET_X2,			// wire 135
  CH_C_OFFSET_X2,			// wire 136
  CH_C_OFFSET_X2,			// wire 137
  CH_C_OFFSET_X2,			// wire 138
  CH_C_OFFSET_X2,			// wire 139
  CH_C_OFFSET_X2,			// wire 140
  CH_C_OFFSET_X2,			// wire 141
  CH_C_OFFSET_X2,			// wire 142
  CH_C_OFFSET_X2,			// wire 143
  CH_C_OFFSET_X2,			// wire 144
  CH_C_OFFSET_X2,			// wire 145
  CH_C_OFFSET_X2,			// wire 146
  CH_C_OFFSET_X2,			// wire 147
  CH_C_OFFSET_X2,			// wire 148
  CH_C_OFFSET_X2,			// wire 149
  CH_C_OFFSET_X2,			// wire 150
  CH_C_OFFSET_X2,			// wire 151
  CH_C_OFFSET_X2,			// wire 152
  CH_C_OFFSET_X2,			// wire 153
  CH_C_OFFSET_X2,			// wire 154
  CH_C_OFFSET_X2,			// wire 155
  CH_C_OFFSET_X2,			// wire 156
  CH_C_OFFSET_X2,			// wire 157
  CH_C_OFFSET_X2,			// wire 158
  CH_C_OFFSET_X2,			// wire 159
  CH_C_OFFSET_X2,			// wire 160
  CH_C_OFFSET_X2,			// wire 161
  CH_C_OFFSET_X2,			// wire 162
  CH_C_OFFSET_X2,			// wire 163
  CH_C_OFFSET_X2,			// wire 164
  CH_C_OFFSET_X2,			// wire 165
  CH_C_OFFSET_X2,			// wire 166
  CH_C_OFFSET_X2,			// wire 167
  CH_C_OFFSET_X2,			// wire 168
  CH_C_OFFSET_X2,			// wire 169
  CH_C_OFFSET_X2,			// wire 170
  CH_C_OFFSET_X2,			// wire 171
  CH_C_OFFSET_X2,			// wire 172
  CH_C_OFFSET_X2,			// wire 173
  CH_C_OFFSET_X2,			// wire 174
  CH_C_OFFSET_X2,			// wire 175
  CH_C_OFFSET_X2,			// wire 176
  CH_C_OFFSET_X2,			// wire 177
  CH_C_OFFSET_X2,			// wire 178
  CH_C_OFFSET_X2,			// wire 179
  CH_C_OFFSET_X2,			// wire 180
  CH_C_OFFSET_X2,			// wire 181
  CH_C_OFFSET_X2,			// wire 182
  CH_C_OFFSET_X2,			// wire 183
  CH_C_OFFSET_X2,			// wire 184
  CH_C_OFFSET_X2,			// wire 185
  CH_C_OFFSET_X2,			// wire 186
  CH_C_OFFSET_X2,			// wire 187
  CH_C_OFFSET_X2,			// wire 188
  CH_C_OFFSET_X2,			// wire 189
  CH_C_OFFSET_X2,			// wire 190
  CH_C_OFFSET_X2,			// wire 191
  CH_C_OFFSET_X2,			// wire 192
  CH_C_OFFSET_X2,			// wire 193
  CH_C_OFFSET_X2,			// wire 194
  CH_C_OFFSET_X2,			// wire 195
  CH_C_OFFSET_X2,			// wire 196
  CH_C_OFFSET_X2,			// wire 197
  CH_C_OFFSET_X2,			// wire 198
  CH_C_OFFSET_X2,			// wire 199
  CH_C_OFFSET_X2,			// wire 200
  CH_C_OFFSET_X2,			// wire 201
  CH_C_OFFSET_X2,			// wire 202
  CH_C_OFFSET_X2,			// wire 203
  CH_C_OFFSET_X2,			// wire 204
  CH_C_OFFSET_X2,			// wire 205
  CH_C_OFFSET_X2,			// wire 206
  CH_C_OFFSET_X2,			// wire 207
  CH_C_OFFSET_X2,			// wire 208
  CH_C_OFFSET_X2,			// wire 209
  CH_C_OFFSET_X2,			// wire 210
  CH_C_OFFSET_X2,			// wire 211
  CH_C_OFFSET_X2,			// wire 212
  CH_C_OFFSET_X2,			// wire 213
  CH_C_OFFSET_X2,			// wire 214
  CH_C_OFFSET_X2,			// wire 215
  CH_C_OFFSET_X2,			// wire 216
  CH_C_OFFSET_X2,			// wire 217
  CH_C_OFFSET_X2,			// wire 218
  CH_C_OFFSET_X2,			// wire 219
  CH_C_OFFSET_X2,			// wire 220
  CH_C_OFFSET_X2,			// wire 221
  CH_C_OFFSET_X2,			// wire 222
  CH_C_OFFSET_X2,			// wire 223
  CH_C_OFFSET_X2,			// wire 224
  CH_C_OFFSET_X2,			// wire 225
  CH_C_OFFSET_X2,			// wire 226
  CH_C_OFFSET_X2,			// wire 227
  CH_C_OFFSET_X2,			// wire 228
  CH_C_OFFSET_X2,			// wire 229
  CH_C_OFFSET_X2,			// wire 230
  CH_C_OFFSET_X2,			// wire 231
  CH_C_OFFSET_X2,			// wire 232
  CH_C_OFFSET_X2,			// wire 233
  CH_C_OFFSET_X2,			// wire 234
  CH_C_OFFSET_X2,			// wire 235
  CH_C_OFFSET_X2,			// wire 236
  CH_C_OFFSET_X2,			// wire 237
  CH_C_OFFSET_X2,			// wire 238
  CH_C_OFFSET_X2,			// wire 239
  CH_C_OFFSET_X2,			// wire 240
  CH_C_OFFSET_X2,			// wire 241
  CH_C_OFFSET_X2,			// wire 242
  CH_C_OFFSET_X2,			// wire 243
  CH_C_OFFSET_X2,			// wire 244
  CH_C_OFFSET_X2,			// wire 245
  CH_C_OFFSET_X2,			// wire 246
  CH_C_OFFSET_X2,			// wire 247
  CH_C_OFFSET_X2,			// wire 248
  CH_C_OFFSET_X2,			// wire 249
  CH_C_OFFSET_X2,			// wire 250
  CH_C_OFFSET_X2,			// wire 251
  CH_C_OFFSET_X2,			// wire 252
  CH_C_OFFSET_X2,			// wire 253
  CH_C_OFFSET_X2,			// wire 254
  CH_C_OFFSET_X2,			// wire 255
  CH_C_OFFSET_X2,			// wire 256
  CH_C_OFFSET_X2,			// wire 257
  CH_C_OFFSET_X2,			// wire 258
  CH_C_OFFSET_X2,			// wire 259
  CH_C_OFFSET_X2,			// wire 260
  CH_C_OFFSET_X2,			// wire 261
  CH_C_OFFSET_X2,			// wire 262
  CH_C_OFFSET_X2,			// wire 263
  CH_C_OFFSET_X2,			// wire 264
  CH_C_OFFSET_X2,			// wire 265
  CH_C_OFFSET_X2,			// wire 266
  CH_C_OFFSET_X2,			// wire 267
  CH_C_OFFSET_X2,			// wire 268
  CH_C_OFFSET_X2,			// wire 269
  CH_C_OFFSET_X2,			// wire 270
  CH_C_OFFSET_X2,			// wire 271
  CH_C_OFFSET_X2,			// wire 272
  CH_C_OFFSET_X2,			// wire 273
  CH_C_OFFSET_X2,			// wire 274
  CH_C_OFFSET_X2,			// wire 275
  CH_C_OFFSET_X2,			// wire 276
  CH_C_OFFSET_X2,			// wire 277
  CH_C_OFFSET_X2,			// wire 278
  CH_C_OFFSET_X2,			// wire 279
  CH_C_OFFSET_X2,			// wire 280
  CH_C_OFFSET_X2,			// wire 281
  CH_C_OFFSET_X2,			// wire 282
  CH_C_OFFSET_X2,			// wire 283
  CH_C_OFFSET_X2,			// wire 284
  CH_C_OFFSET_X2,			// wire 285
  CH_C_OFFSET_X2,			// wire 286
  CH_C_OFFSET_X2,			// wire 287
  CH_C_OFFSET_X2,			// wire 288
  CH_C_OFFSET_X2,			// wire 289
  CH_C_OFFSET_X2,			// wire 290
  CH_C_OFFSET_X2,			// wire 291
  CH_C_OFFSET_X2,			// wire 292
  CH_C_OFFSET_X2,			// wire 293
  CH_C_OFFSET_X2,			// wire 294
  CH_C_OFFSET_X2,			// wire 295
  CH_C_OFFSET_X2,			// wire 296
  CH_C_OFFSET_X2,			// wire 297
  CH_C_OFFSET_X2,			// wire 298
  CH_C_OFFSET_X2,			// wire 299
  CH_C_OFFSET_X2,			// wire 300
  CH_C_OFFSET_X2,			// wire 301
  CH_C_OFFSET_X2,			// wire 302
  CH_C_OFFSET_X2,			// wire 303
  CH_C_OFFSET_X2,			// wire 304
  CH_C_OFFSET_X2,			// wire 305
  CH_C_OFFSET_X2,			// wire 306
  CH_C_OFFSET_X2,			// wire 307
  CH_C_OFFSET_X2,			// wire 308
  CH_C_OFFSET_X2,			// wire 309
  CH_C_OFFSET_X2,			// wire 310
  CH_C_OFFSET_X2,			// wire 311
  CH_C_OFFSET_X2,			// wire 312
  CH_C_OFFSET_X2,			// wire 313
  CH_C_OFFSET_X2,			// wire 314
  CH_C_OFFSET_X2,			// wire 315
  CH_C_OFFSET_X2,			// wire 316
  CH_C_OFFSET_X2,			// wire 317
  CH_C_OFFSET_X2,			// wire 318
  CH_C_OFFSET_X2,			// wire 319
  CH_C_OFFSET_X2,			// wire 320
  CH_C_OFFSET_X2,			// wire 321
  CH_C_OFFSET_X2,			// wire 322
  CH_C_OFFSET_X2,			// wire 323
  CH_C_OFFSET_X2,			// wire 324
  CH_C_OFFSET_X2,			// wire 325
  CH_C_OFFSET_X2,			// wire 326
  CH_C_OFFSET_X2,			// wire 327
  CH_C_OFFSET_X2,			// wire 328
  CH_C_OFFSET_X2,			// wire 329
  CH_C_OFFSET_X2,			// wire 330
  CH_C_OFFSET_X2,			// wire 331
  CH_C_OFFSET_X2,			// wire 332
  CH_C_OFFSET_X2,			// wire 333
  CH_C_OFFSET_X2,			// wire 334
  CH_C_OFFSET_X2,			// wire 335
  CH_C_OFFSET_X2,			// wire 336
  CH_C_OFFSET_X2,			// wire 337
  CH_C_OFFSET_X2,			// wire 338
  CH_C_OFFSET_X2,			// wire 339
  CH_C_OFFSET_X2,			// wire 340
  CH_C_OFFSET_X2,			// wire 341
  CH_C_OFFSET_X2,			// wire 342
  CH_C_OFFSET_X2,			// wire 343
  CH_C_OFFSET_X2,			// wire 344
  CH_C_OFFSET_X2,			// wire 345
  CH_C_OFFSET_X2,			// wire 346
  CH_C_OFFSET_X2,			// wire 347
  CH_C_OFFSET_X2,			// wire 348
  CH_C_OFFSET_X2,			// wire 349
  CH_C_OFFSET_X2,			// wire 350
  CH_C_OFFSET_X2,			// wire 351
  CH_C_OFFSET_X2,			// wire 352
  CH_C_OFFSET_X2,			// wire 353
  CH_C_OFFSET_X2,			// wire 354
  CH_C_OFFSET_X2,			// wire 355
  CH_C_OFFSET_X2,			// wire 356
  CH_C_OFFSET_X2,			// wire 357
  CH_C_OFFSET_X2,			// wire 358
  CH_C_OFFSET_X2,			// wire 359
  CH_C_OFFSET_X2,			// wire 360
  CH_C_OFFSET_X2,			// wire 361
  CH_C_OFFSET_X2,			// wire 362
  CH_C_OFFSET_X2,			// wire 363
  CH_C_OFFSET_X2,			// wire 364
  CH_C_OFFSET_X2,			// wire 365
  CH_C_OFFSET_X2,			// wire 366
  CH_C_OFFSET_X2,			// wire 367
  CH_C_OFFSET_X2,			// wire 368
  CH_C_OFFSET_X2,			// wire 369
  CH_C_OFFSET_X2,			// wire 370
  CH_C_OFFSET_X2,			// wire 371
  CH_C_OFFSET_X2,			// wire 372
  CH_C_OFFSET_X2,			// wire 373
  CH_C_OFFSET_X2,			// wire 374
  CH_C_OFFSET_X2,			// wire 375
  CH_C_OFFSET_X2,			// wire 376
  CH_C_OFFSET_X2,			// wire 377
  CH_C_OFFSET_X2,			// wire 378
  CH_C_OFFSET_X2,			// wire 379
  CH_C_OFFSET_X2,			// wire 380
  CH_C_OFFSET_X2,			// wire 381
  CH_C_OFFSET_X2,			// wire 382
  CH_C_OFFSET_X2,			// wire 383
  CH_C_OFFSET_X2,			// wire 384
  CH_C_OFFSET_X2,			// wire 385
  CH_C_OFFSET_X2,			// wire 386
  CH_C_OFFSET_X2,			// wire 387
  CH_C_OFFSET_X2,			// wire 388
  CH_C_OFFSET_X2,			// wire 389
  CH_C_OFFSET_X2,			// wire 390
  CH_C_OFFSET_X2,			// wire 391
  CH_C_OFFSET_X2,			// wire 392
  CH_C_OFFSET_X2,			// wire 393
  CH_C_OFFSET_X2,			// wire 394
  CH_C_OFFSET_X2,			// wire 395
  CH_C_OFFSET_X2,			// wire 396
  CH_C_OFFSET_X2,			// wire 397
  CH_C_OFFSET_X2,			// wire 398
  CH_C_OFFSET_X2,			// wire 399
  CH_C_OFFSET_X2,			// wire 400
  CH_C_OFFSET_X2,			// wire 401
  CH_C_OFFSET_X2,			// wire 402
  CH_C_OFFSET_X2,			// wire 403
  CH_C_OFFSET_X2,			// wire 404
  CH_C_OFFSET_X2,			// wire 405
  CH_C_OFFSET_X2,			// wire 406
  CH_C_OFFSET_X2,			// wire 407
  CH_C_OFFSET_X2,			// wire 408
  CH_C_OFFSET_X2,			// wire 409
  CH_C_OFFSET_X2,			// wire 410
  CH_C_OFFSET_X2,			// wire 411
  CH_C_OFFSET_X2,			// wire 412
  CH_C_OFFSET_X2,			// wire 413
  CH_C_OFFSET_X2,			// wire 414
  CH_C_OFFSET_X2,			// wire 415
  CH_C_OFFSET_X2,			// wire 416
  CH_C_OFFSET_X2,			// wire 417
  CH_C_OFFSET_X2,			// wire 418
  CH_C_OFFSET_X2,			// wire 419
  CH_C_OFFSET_X2,			// wire 420
  CH_C_OFFSET_X2,			// wire 421
  CH_C_OFFSET_X2,			// wire 422
  CH_C_OFFSET_X2,			// wire 423
  CH_C_OFFSET_X2,			// wire 424
  CH_C_OFFSET_X2,			// wire 425
  CH_C_OFFSET_X2,			// wire 426
  CH_C_OFFSET_X2,			// wire 427
  CH_C_OFFSET_X2,			// wire 428
  CH_C_OFFSET_X2,			// wire 429
  CH_C_OFFSET_X2,			// wire 430
  CH_C_OFFSET_X2,			// wire 431
  CH_C_OFFSET_X2,			// wire 432
  CH_C_OFFSET_X2,			// wire 433
  CH_C_OFFSET_X2,			// wire 434
  CH_C_OFFSET_X2,			// wire 435
  CH_C_OFFSET_X2,			// wire 436
  CH_C_OFFSET_X2,			// wire 437
  CH_C_OFFSET_X2,			// wire 438
  CH_C_OFFSET_X2,			// wire 439
  CH_C_OFFSET_X2,			// wire 440
  CH_C_OFFSET_X2,			// wire 441
  CH_C_OFFSET_X2,			// wire 442
  CH_C_OFFSET_X2,			// wire 443
  CH_C_OFFSET_X2,			// wire 444
  CH_C_OFFSET_X2,			// wire 445
  CH_C_OFFSET_X2,			// wire 446
  CH_C_OFFSET_X2,			// wire 447
  CH_C_OFFSET_X2			// wire 448
};

static short
offset_cs2[CH_C_MAX_S2] = {
  CH_C_OFFSET_S2,			// wire 1
  CH_C_OFFSET_S2,			// wire 2
  CH_C_OFFSET_S2,			// wire 3
  CH_C_OFFSET_S2,			// wire 4
  CH_C_OFFSET_S2,			// wire 5
  CH_C_OFFSET_S2,			// wire 6
  CH_C_OFFSET_S2,			// wire 7
  CH_C_OFFSET_S2,			// wire 8
  CH_C_OFFSET_S2,			// wire 9
  CH_C_OFFSET_S2,			// wire 10
  CH_C_OFFSET_S2,			// wire 11
  CH_C_OFFSET_S2,			// wire 12
  CH_C_OFFSET_S2,			// wire 13
  CH_C_OFFSET_S2,			// wire 14
  CH_C_OFFSET_S2,			// wire 15
  CH_C_OFFSET_S2,			// wire 16
  CH_C_OFFSET_S2,			// wire 17
  CH_C_OFFSET_S2,			// wire 18
  CH_C_OFFSET_S2,			// wire 19
  CH_C_OFFSET_S2,			// wire 20
  CH_C_OFFSET_S2,			// wire 21
  CH_C_OFFSET_S2,			// wire 22
  CH_C_OFFSET_S2,			// wire 23
  CH_C_OFFSET_S2,			// wire 24
  CH_C_OFFSET_S2,			// wire 25
  CH_C_OFFSET_S2,			// wire 26
  CH_C_OFFSET_S2,			// wire 27
  CH_C_OFFSET_S2,			// wire 28
  CH_C_OFFSET_S2,			// wire 29
  CH_C_OFFSET_S2,			// wire 30
  CH_C_OFFSET_S2,			// wire 31
  CH_C_OFFSET_S2,			// wire 32
  CH_C_OFFSET_S2,			// wire 33
  CH_C_OFFSET_S2,			// wire 34
  CH_C_OFFSET_S2,			// wire 35
  CH_C_OFFSET_S2,			// wire 36
  CH_C_OFFSET_S2,			// wire 37
  CH_C_OFFSET_S2,			// wire 38
  CH_C_OFFSET_S2,			// wire 39
  CH_C_OFFSET_S2,			// wire 40
  CH_C_OFFSET_S2,			// wire 41
  CH_C_OFFSET_S2,			// wire 42
  CH_C_OFFSET_S2,			// wire 43
  CH_C_OFFSET_S2,			// wire 44
  CH_C_OFFSET_S2,			// wire 45
  CH_C_OFFSET_S2,			// wire 46
  CH_C_OFFSET_S2,			// wire 47
  CH_C_OFFSET_S2,			// wire 48
  CH_C_OFFSET_S2,			// wire 49
  CH_C_OFFSET_S2,			// wire 50
  CH_C_OFFSET_S2,			// wire 51
  CH_C_OFFSET_S2,			// wire 52
  CH_C_OFFSET_S2,			// wire 53
  CH_C_OFFSET_S2,			// wire 54
  CH_C_OFFSET_S2,			// wire 55
  CH_C_OFFSET_S2,			// wire 56
  CH_C_OFFSET_S2,			// wire 57
  CH_C_OFFSET_S2,			// wire 58
  CH_C_OFFSET_S2,			// wire 59
  CH_C_OFFSET_S2,			// wire 60
  CH_C_OFFSET_S2,			// wire 61
  CH_C_OFFSET_S2,			// wire 62
  CH_C_OFFSET_S2,			// wire 63
  CH_C_OFFSET_S2,			// wire 64
  CH_C_OFFSET_S2,			// wire 65
  CH_C_OFFSET_S2,			// wire 66
  CH_C_OFFSET_S2,			// wire 67
  CH_C_OFFSET_S2,			// wire 68
  CH_C_OFFSET_S2,			// wire 69
  CH_C_OFFSET_S2,			// wire 70
  CH_C_OFFSET_S2,			// wire 71
  CH_C_OFFSET_S2,			// wire 72
  CH_C_OFFSET_S2,			// wire 73
  CH_C_OFFSET_S2,			// wire 74
  CH_C_OFFSET_S2,			// wire 75
  CH_C_OFFSET_S2,			// wire 76
  CH_C_OFFSET_S2,			// wire 77
  CH_C_OFFSET_S2,			// wire 78
  CH_C_OFFSET_S2,			// wire 79
  CH_C_OFFSET_S2,			// wire 80
  CH_C_OFFSET_S2,			// wire 81
  CH_C_OFFSET_S2,			// wire 82
  CH_C_OFFSET_S2,			// wire 83
  CH_C_OFFSET_S2,			// wire 84
  CH_C_OFFSET_S2,			// wire 85
  CH_C_OFFSET_S2,			// wire 86
  CH_C_OFFSET_S2,			// wire 87
  CH_C_OFFSET_S2,			// wire 88
  CH_C_OFFSET_S2,			// wire 89
  CH_C_OFFSET_S2,			// wire 90
  CH_C_OFFSET_S2,			// wire 91
  CH_C_OFFSET_S2,			// wire 92
  CH_C_OFFSET_S2,			// wire 93
  CH_C_OFFSET_S2,			// wire 94
  CH_C_OFFSET_S2,			// wire 95
  CH_C_OFFSET_S2,			// wire 96
  CH_C_OFFSET_S2,			// wire 97
  CH_C_OFFSET_S2,			// wire 98
  CH_C_OFFSET_S2,			// wire 99
  CH_C_OFFSET_S2,			// wire 100
  CH_C_OFFSET_S2,			// wire 101
  CH_C_OFFSET_S2,			// wire 102
  CH_C_OFFSET_S2,			// wire 103
  CH_C_OFFSET_S2,			// wire 104
  CH_C_OFFSET_S2,			// wire 105
  CH_C_OFFSET_S2,			// wire 106
  CH_C_OFFSET_S2,			// wire 107
  CH_C_OFFSET_S2,			// wire 108
  CH_C_OFFSET_S2,			// wire 109
  CH_C_OFFSET_S2,			// wire 110
  CH_C_OFFSET_S2,			// wire 111
  CH_C_OFFSET_S2,			// wire 112
  CH_C_OFFSET_S2,			// wire 113
  CH_C_OFFSET_S2,			// wire 114
  CH_C_OFFSET_S2,			// wire 115
  CH_C_OFFSET_S2,			// wire 116
  CH_C_OFFSET_S2,			// wire 117
  CH_C_OFFSET_S2,			// wire 118
  CH_C_OFFSET_S2,			// wire 119
  CH_C_OFFSET_S2,			// wire 120
  CH_C_OFFSET_S2,			// wire 121
  CH_C_OFFSET_S2,			// wire 122
  CH_C_OFFSET_S2,			// wire 123
  CH_C_OFFSET_S2,			// wire 124
  CH_C_OFFSET_S2,			// wire 125
  CH_C_OFFSET_S2,			// wire 126
  CH_C_OFFSET_S2,			// wire 127
  CH_C_OFFSET_S2,			// wire 128
  CH_C_OFFSET_S2,			// wire 129
  CH_C_OFFSET_S2,			// wire 130
  CH_C_OFFSET_S2,			// wire 131
  CH_C_OFFSET_S2,			// wire 132
  CH_C_OFFSET_S2,			// wire 133
  CH_C_OFFSET_S2,			// wire 134
  CH_C_OFFSET_S2,			// wire 135
  CH_C_OFFSET_S2,			// wire 136
  CH_C_OFFSET_S2,			// wire 137
  CH_C_OFFSET_S2,			// wire 138
  CH_C_OFFSET_S2,			// wire 139
  CH_C_OFFSET_S2,			// wire 140
  CH_C_OFFSET_S2,			// wire 141
  CH_C_OFFSET_S2,			// wire 142
  CH_C_OFFSET_S2,			// wire 143
  CH_C_OFFSET_S2,			// wire 144
  CH_C_OFFSET_S2,			// wire 145
  CH_C_OFFSET_S2,			// wire 146
  CH_C_OFFSET_S2,			// wire 147
  CH_C_OFFSET_S2,			// wire 148
  CH_C_OFFSET_S2,			// wire 149
  CH_C_OFFSET_S2,			// wire 150
  CH_C_OFFSET_S2,			// wire 151
  CH_C_OFFSET_S2,			// wire 152
  CH_C_OFFSET_S2,			// wire 153
  CH_C_OFFSET_S2,			// wire 154
  CH_C_OFFSET_S2,			// wire 155
  CH_C_OFFSET_S2,			// wire 156
  CH_C_OFFSET_S2,			// wire 157
  CH_C_OFFSET_S2,			// wire 158
  CH_C_OFFSET_S2,			// wire 159
  CH_C_OFFSET_S2,			// wire 160
  CH_C_OFFSET_S2,			// wire 161
  CH_C_OFFSET_S2,			// wire 162
  CH_C_OFFSET_S2,			// wire 163
  CH_C_OFFSET_S2,			// wire 164
  CH_C_OFFSET_S2,			// wire 165
  CH_C_OFFSET_S2,			// wire 166
  CH_C_OFFSET_S2,			// wire 167
  CH_C_OFFSET_S2,			// wire 168
  CH_C_OFFSET_S2,			// wire 169
  CH_C_OFFSET_S2,			// wire 170
  CH_C_OFFSET_S2,			// wire 171
  CH_C_OFFSET_S2,			// wire 172
  CH_C_OFFSET_S2,			// wire 173
  CH_C_OFFSET_S2,			// wire 174
  CH_C_OFFSET_S2,			// wire 175
  CH_C_OFFSET_S2,			// wire 176
  CH_C_OFFSET_S2,			// wire 177
  CH_C_OFFSET_S2,			// wire 178
  CH_C_OFFSET_S2,			// wire 179
  CH_C_OFFSET_S2,			// wire 180
  CH_C_OFFSET_S2,			// wire 181
  CH_C_OFFSET_S2,			// wire 182
  CH_C_OFFSET_S2,			// wire 183
  CH_C_OFFSET_S2,			// wire 184
  CH_C_OFFSET_S2,			// wire 185
  CH_C_OFFSET_S2,			// wire 186
  CH_C_OFFSET_S2,			// wire 187
  CH_C_OFFSET_S2,			// wire 188
  CH_C_OFFSET_S2,			// wire 189
  CH_C_OFFSET_S2,			// wire 190
  CH_C_OFFSET_S2,			// wire 191
  CH_C_OFFSET_S2,			// wire 192
  CH_C_OFFSET_S2,			// wire 193
  CH_C_OFFSET_S2,			// wire 194
  CH_C_OFFSET_S2,			// wire 195
  CH_C_OFFSET_S2,			// wire 196
  CH_C_OFFSET_S2,			// wire 197
  CH_C_OFFSET_S2,			// wire 198
  CH_C_OFFSET_S2,			// wire 199
  CH_C_OFFSET_S2,			// wire 200
  CH_C_OFFSET_S2,			// wire 201
  CH_C_OFFSET_S2,			// wire 202
  CH_C_OFFSET_S2,			// wire 203
  CH_C_OFFSET_S2,			// wire 204
  CH_C_OFFSET_S2,			// wire 205
  CH_C_OFFSET_S2,			// wire 206
  CH_C_OFFSET_S2,			// wire 207
  CH_C_OFFSET_S2,			// wire 208
  CH_C_OFFSET_S2,			// wire 209
  CH_C_OFFSET_S2,			// wire 210
  CH_C_OFFSET_S2,			// wire 211
  CH_C_OFFSET_S2,			// wire 212
  CH_C_OFFSET_S2,			// wire 213
  CH_C_OFFSET_S2,			// wire 214
  CH_C_OFFSET_S2,			// wire 215
  CH_C_OFFSET_S2,			// wire 216
  CH_C_OFFSET_S2,			// wire 217
  CH_C_OFFSET_S2,			// wire 218
  CH_C_OFFSET_S2,			// wire 219
  CH_C_OFFSET_S2,			// wire 220
  CH_C_OFFSET_S2,			// wire 221
  CH_C_OFFSET_S2,			// wire 222
  CH_C_OFFSET_S2,			// wire 223
  CH_C_OFFSET_S2,			// wire 224
  CH_C_OFFSET_S2,			// wire 225
  CH_C_OFFSET_S2,			// wire 226
  CH_C_OFFSET_S2,			// wire 227
  CH_C_OFFSET_S2,			// wire 228
  CH_C_OFFSET_S2,			// wire 229
  CH_C_OFFSET_S2,			// wire 230
  CH_C_OFFSET_S2,			// wire 231
  CH_C_OFFSET_S2,			// wire 232
  CH_C_OFFSET_S2,			// wire 233
  CH_C_OFFSET_S2,			// wire 234
  CH_C_OFFSET_S2,			// wire 235
  CH_C_OFFSET_S2,			// wire 236
  CH_C_OFFSET_S2,			// wire 237
  CH_C_OFFSET_S2,			// wire 238
  CH_C_OFFSET_S2,			// wire 239
  CH_C_OFFSET_S2,			// wire 240
  CH_C_OFFSET_S2,			// wire 241
  CH_C_OFFSET_S2,			// wire 242
  CH_C_OFFSET_S2,			// wire 243
  CH_C_OFFSET_S2,			// wire 244
  CH_C_OFFSET_S2,			// wire 245
  CH_C_OFFSET_S2,			// wire 246
  CH_C_OFFSET_S2,			// wire 247
  CH_C_OFFSET_S2,			// wire 248
  CH_C_OFFSET_S2,			// wire 249
  CH_C_OFFSET_S2,			// wire 250
  CH_C_OFFSET_S2,			// wire 251
  CH_C_OFFSET_S2,			// wire 252
  CH_C_OFFSET_S2,			// wire 253
  CH_C_OFFSET_S2,			// wire 254
  CH_C_OFFSET_S2,			// wire 255
  CH_C_OFFSET_S2,			// wire 256
  CH_C_OFFSET_S2,			// wire 257
  CH_C_OFFSET_S2,			// wire 258
  CH_C_OFFSET_S2,			// wire 259
  CH_C_OFFSET_S2,			// wire 260
  CH_C_OFFSET_S2,			// wire 261
  CH_C_OFFSET_S2,			// wire 262
  CH_C_OFFSET_S2,			// wire 263
  CH_C_OFFSET_S2,			// wire 264
  CH_C_OFFSET_S2,			// wire 265
  CH_C_OFFSET_S2,			// wire 266
  CH_C_OFFSET_S2,			// wire 267
  CH_C_OFFSET_S2,			// wire 268
  CH_C_OFFSET_S2,			// wire 269
  CH_C_OFFSET_S2,			// wire 270
  CH_C_OFFSET_S2,			// wire 271
  CH_C_OFFSET_S2,			// wire 272
  CH_C_OFFSET_S2,			// wire 273
  CH_C_OFFSET_S2,			// wire 274
  CH_C_OFFSET_S2,			// wire 275
  CH_C_OFFSET_S2,			// wire 276
  CH_C_OFFSET_S2,			// wire 277
  CH_C_OFFSET_S2,			// wire 278
  CH_C_OFFSET_S2,			// wire 279
  CH_C_OFFSET_S2,			// wire 280
  CH_C_OFFSET_S2,			// wire 281
  CH_C_OFFSET_S2,			// wire 282
  CH_C_OFFSET_S2,			// wire 283
  CH_C_OFFSET_S2,			// wire 284
  CH_C_OFFSET_S2,			// wire 285
  CH_C_OFFSET_S2,			// wire 286
  CH_C_OFFSET_S2,			// wire 287
  CH_C_OFFSET_S2,			// wire 288
  CH_C_OFFSET_S2,			// wire 289
  CH_C_OFFSET_S2,			// wire 290
  CH_C_OFFSET_S2,			// wire 291
  CH_C_OFFSET_S2,			// wire 292
  CH_C_OFFSET_S2,			// wire 293
  CH_C_OFFSET_S2,			// wire 294
  CH_C_OFFSET_S2,			// wire 295
  CH_C_OFFSET_S2,			// wire 296
  CH_C_OFFSET_S2,			// wire 297
  CH_C_OFFSET_S2,			// wire 298
  CH_C_OFFSET_S2,			// wire 299
  CH_C_OFFSET_S2,			// wire 300
  CH_C_OFFSET_S2,			// wire 301
  CH_C_OFFSET_S2,			// wire 302
  CH_C_OFFSET_S2,			// wire 303
  CH_C_OFFSET_S2,			// wire 304
  CH_C_OFFSET_S2,			// wire 305
  CH_C_OFFSET_S2,			// wire 306
  CH_C_OFFSET_S2,			// wire 307
  CH_C_OFFSET_S2,			// wire 308
  CH_C_OFFSET_S2,			// wire 309
  CH_C_OFFSET_S2,			// wire 310
  CH_C_OFFSET_S2,			// wire 311
  CH_C_OFFSET_S2,			// wire 312
  CH_C_OFFSET_S2,			// wire 313
  CH_C_OFFSET_S2,			// wire 314
  CH_C_OFFSET_S2,			// wire 315
  CH_C_OFFSET_S2,			// wire 316
  CH_C_OFFSET_S2,			// wire 317
  CH_C_OFFSET_S2,			// wire 318
  CH_C_OFFSET_S2,			// wire 319
  CH_C_OFFSET_S2,			// wire 320
  CH_C_OFFSET_S2,			// wire 321
  CH_C_OFFSET_S2,			// wire 322
  CH_C_OFFSET_S2,			// wire 323
  CH_C_OFFSET_S2,			// wire 324
  CH_C_OFFSET_S2,			// wire 325
  CH_C_OFFSET_S2,			// wire 326
  CH_C_OFFSET_S2,			// wire 327
  CH_C_OFFSET_S2,			// wire 328
  CH_C_OFFSET_S2,			// wire 329
  CH_C_OFFSET_S2,			// wire 330
  CH_C_OFFSET_S2,			// wire 331
  CH_C_OFFSET_S2,			// wire 332
  CH_C_OFFSET_S2,			// wire 333
  CH_C_OFFSET_S2,			// wire 334
  CH_C_OFFSET_S2,			// wire 335
  CH_C_OFFSET_S2,			// wire 336
  CH_C_OFFSET_S2,			// wire 337
  CH_C_OFFSET_S2,			// wire 338
  CH_C_OFFSET_S2,			// wire 339
  CH_C_OFFSET_S2,			// wire 340
  CH_C_OFFSET_S2,			// wire 341
  CH_C_OFFSET_S2,			// wire 342
  CH_C_OFFSET_S2,			// wire 343
  CH_C_OFFSET_S2,			// wire 344
  CH_C_OFFSET_S2,			// wire 345
  CH_C_OFFSET_S2,			// wire 346
  CH_C_OFFSET_S2,			// wire 347
  CH_C_OFFSET_S2,			// wire 348
  CH_C_OFFSET_S2,			// wire 349
  CH_C_OFFSET_S2,			// wire 350
  CH_C_OFFSET_S2,			// wire 351
  CH_C_OFFSET_S2			// wire 352
};

static float
position_cx1[CH_C_MAX_X1] = {
  -797.60,		// x1 wire 1
  -792.60,		// x1 wire 2
  -787.60,		// x1 wire 3
  -782.60,		// x1 wire 4
  -777.60,		// x1 wire 5
  -772.60,		// x1 wire 6
  -767.60,		// x1 wire 7
  -762.60,		// x1 wire 8
  -757.60,		// x1 wire 9
  -752.60,		// x1 wire 10
  -747.60,		// x1 wire 11
  -742.60,		// x1 wire 12
  -737.60,		// x1 wire 13
  -732.60,		// x1 wire 14
  -727.60,		// x1 wire 15
  -722.60,		// x1 wire 16
  -717.60,		// x1 wire 17
  -712.60,		// x1 wire 18
  -707.60,		// x1 wire 19
  -702.60,		// x1 wire 20
  -697.60,		// x1 wire 21
  -692.60,		// x1 wire 22
  -687.60,		// x1 wire 23
  -682.60,		// x1 wire 24
  -677.60,		// x1 wire 25
  -672.60,		// x1 wire 26
  -667.60,		// x1 wire 27
  -662.60,		// x1 wire 28
  -657.60,		// x1 wire 29
  -652.60,		// x1 wire 30
  -647.60,		// x1 wire 31
  -642.60,		// x1 wire 32
  -637.60,		// x1 wire 33
  -632.60,		// x1 wire 34
  -627.60,		// x1 wire 35
  -622.60,		// x1 wire 36
  -617.60,		// x1 wire 37
  -612.60,		// x1 wire 38
  -607.60,		// x1 wire 39
  -602.60,		// x1 wire 40
  -597.60,		// x1 wire 41
  -592.60,		// x1 wire 42
  -587.60,		// x1 wire 43
  -582.60,		// x1 wire 44
  -577.60,		// x1 wire 45
  -572.60,		// x1 wire 46
  -567.60,		// x1 wire 47
  -562.60,		// x1 wire 48
  -557.60,		// x1 wire 49
  -552.60,		// x1 wire 50
  -547.60,		// x1 wire 51
  -542.60,		// x1 wire 52
  -537.60,		// x1 wire 53
  -532.60,		// x1 wire 54
  -527.60,		// x1 wire 55
  -522.60,		// x1 wire 56
  -517.60,		// x1 wire 57
  -512.60,		// x1 wire 58
  -507.60,		// x1 wire 59
  -502.60,		// x1 wire 60
  -497.60,		// x1 wire 61
  -492.60,		// x1 wire 62
  -487.60,		// x1 wire 63
  -482.60,		// x1 wire 64
  -477.60,		// x1 wire 65
  -472.60,		// x1 wire 66
  -467.60,		// x1 wire 67
  -462.60,		// x1 wire 68
  -457.60,		// x1 wire 69
  -452.60,		// x1 wire 70
  -447.60,		// x1 wire 71
  -442.60,		// x1 wire 72
  -437.60,		// x1 wire 73
  -432.60,		// x1 wire 74
  -427.60,		// x1 wire 75
  -422.60,		// x1 wire 76
  -417.60,		// x1 wire 77
  -412.60,		// x1 wire 78
  -407.60,		// x1 wire 79
  -402.60,		// x1 wire 80
  -397.60,		// x1 wire 81
  -392.60,		// x1 wire 82
  -387.60,		// x1 wire 83
  -382.60,		// x1 wire 84
  -377.60,		// x1 wire 85
  -372.60,		// x1 wire 86
  -367.60,		// x1 wire 87
  -362.60,		// x1 wire 88
  -357.60,		// x1 wire 89
  -352.60,		// x1 wire 90
  -347.60,		// x1 wire 91
  -342.60,		// x1 wire 92
  -337.60,		// x1 wire 93
  -332.60,		// x1 wire 94
  -327.60,		// x1 wire 95
  -322.60,		// x1 wire 96
  -317.60,		// x1 wire 97
  -312.60,		// x1 wire 98
  -307.60,		// x1 wire 99
  -302.60,		// x1 wire 100
  -297.60,		// x1 wire 101
  -292.60,		// x1 wire 102
  -287.60,		// x1 wire 103
  -282.60,		// x1 wire 104
  -277.60,		// x1 wire 105
  -272.60,		// x1 wire 106
  -267.60,		// x1 wire 107
  -262.60,		// x1 wire 108
  -257.60,		// x1 wire 109
  -252.60,		// x1 wire 110
  -247.60,		// x1 wire 111
  -242.60,		// x1 wire 112
  -237.60,		// x1 wire 113
  -232.60,		// x1 wire 114
  -227.60,		// x1 wire 115
  -222.60,		// x1 wire 116
  -217.60,		// x1 wire 117
  -212.60,		// x1 wire 118
  -207.60,		// x1 wire 119
  -202.60,		// x1 wire 120
  -197.60,		// x1 wire 121
  -192.60,		// x1 wire 122
  -187.60,		// x1 wire 123
  -182.60,		// x1 wire 124
  -177.60,		// x1 wire 125
  -172.60,		// x1 wire 126
  -167.60,		// x1 wire 127
  -162.60,		// x1 wire 128
  -157.60,		// x1 wire 129
  -152.60,		// x1 wire 130
  -147.60,		// x1 wire 131
  -142.60,		// x1 wire 132
  -137.60,		// x1 wire 133
  -132.60,		// x1 wire 134
  -127.60,		// x1 wire 135
  -122.60,		// x1 wire 136
  -117.60,		// x1 wire 137
  -112.60,		// x1 wire 138
  -107.60,		// x1 wire 139
  -102.60,		// x1 wire 140
   -97.60,		// x1 wire 141
   -92.60,		// x1 wire 142
   -87.60,		// x1 wire 143
   -82.60,		// x1 wire 144
   -77.60,		// x1 wire 145
   -72.60,		// x1 wire 146
   -67.60,		// x1 wire 147
   -62.60,		// x1 wire 148
   -57.60,		// x1 wire 149
   -52.60,		// x1 wire 150
   -47.60,		// x1 wire 151
   -42.60,		// x1 wire 152
   -37.60,		// x1 wire 153
   -32.60,		// x1 wire 154
   -27.60,		// x1 wire 155
   -22.60,		// x1 wire 156
   -17.60,		// x1 wire 157
   -12.60,		// x1 wire 158
    -7.60,		// x1 wire 159
    -2.60,		// x1 wire 160
     2.40,		// x1 wire 161
     7.40,		// x1 wire 162
    12.40,		// x1 wire 163
    17.40,		// x1 wire 164
    22.40,		// x1 wire 165
    27.40,		// x1 wire 166
    32.40,		// x1 wire 167
    37.40,		// x1 wire 168
    42.40,		// x1 wire 169
    47.40,		// x1 wire 170
    52.40,		// x1 wire 171
    57.40,		// x1 wire 172
    62.40,		// x1 wire 173
    67.40,		// x1 wire 174
    72.40,		// x1 wire 175
    77.40,		// x1 wire 176
    82.40,		// x1 wire 177
    87.40,		// x1 wire 178
    92.40,		// x1 wire 179
    97.40,		// x1 wire 180
   102.40,		// x1 wire 181
   107.40,		// x1 wire 182
   112.40,		// x1 wire 183
   117.40,		// x1 wire 184
   122.40,		// x1 wire 185
   127.40,		// x1 wire 186
   132.40,		// x1 wire 187
   137.40,		// x1 wire 188
   142.40,		// x1 wire 189
   147.40,		// x1 wire 190
   152.40,		// x1 wire 191
   157.40,		// x1 wire 192
   162.40,		// x1 wire 193
   167.40,		// x1 wire 194
   172.40,		// x1 wire 195
   177.40,		// x1 wire 196
   182.40,		// x1 wire 197
   187.40,		// x1 wire 198
   192.40,		// x1 wire 199
   197.40,		// x1 wire 200
   202.40,		// x1 wire 201
   207.40,		// x1 wire 202
   212.40,		// x1 wire 203
   217.40,		// x1 wire 204
   222.40,		// x1 wire 205
   227.40,		// x1 wire 206
   232.40,		// x1 wire 207
   237.40,		// x1 wire 208
   242.40,		// x1 wire 209
   247.40,		// x1 wire 210
   252.40,		// x1 wire 211
   257.40,		// x1 wire 212
   262.40,		// x1 wire 213
   267.40,		// x1 wire 214
   272.40,		// x1 wire 215
   277.40,		// x1 wire 216
   282.40,		// x1 wire 217
   287.40,		// x1 wire 218
   292.40,		// x1 wire 219
   297.40,		// x1 wire 220
   302.40,		// x1 wire 221
   307.40,		// x1 wire 222
   312.40,		// x1 wire 223
   317.40,		// x1 wire 224
   322.40,		// x1 wire 225
   327.40,		// x1 wire 226
   332.40,		// x1 wire 227
   337.40,		// x1 wire 228
   342.40,		// x1 wire 229
   347.40,		// x1 wire 230
   352.40,		// x1 wire 231
   357.40,		// x1 wire 232
   362.40,		// x1 wire 233
   367.40,		// x1 wire 234
   372.40,		// x1 wire 235
   377.40,		// x1 wire 236
   382.40,		// x1 wire 237
   387.40,		// x1 wire 238
   392.40,		// x1 wire 239
   397.40,		// x1 wire 240
   402.40,		// x1 wire 241
   407.40,		// x1 wire 242
   412.40,		// x1 wire 243
   417.40,		// x1 wire 244
   422.40,		// x1 wire 245
   427.40,		// x1 wire 246
   432.40,		// x1 wire 247
   437.40,		// x1 wire 248
   442.40,		// x1 wire 249
   447.40,		// x1 wire 250
   452.40,		// x1 wire 251
   457.40,		// x1 wire 252
   462.40,		// x1 wire 253
   467.40,		// x1 wire 254
   472.40,		// x1 wire 255
   477.40,		// x1 wire 256
   482.40,		// x1 wire 257
   487.40,		// x1 wire 258
   492.40,		// x1 wire 259
   497.40,		// x1 wire 260
   502.40,		// x1 wire 261
   507.40,		// x1 wire 262
   512.40,		// x1 wire 263
   517.40,		// x1 wire 264
   522.40,		// x1 wire 265
   527.40,		// x1 wire 266
   532.40,		// x1 wire 267
   537.40,		// x1 wire 268
   542.40,		// x1 wire 269
   547.40,		// x1 wire 270
   552.40,		// x1 wire 271
   557.40,		// x1 wire 272
   562.40,		// x1 wire 273
   567.40,		// x1 wire 274
   572.40,		// x1 wire 275
   577.40,		// x1 wire 276
   582.40,		// x1 wire 277
   587.40,		// x1 wire 278
   592.40,		// x1 wire 279
   597.40,		// x1 wire 280
   602.40,		// x1 wire 281
   607.40,		// x1 wire 282
   612.40,		// x1 wire 283
   617.40,		// x1 wire 284
   622.40,		// x1 wire 285
   627.40,		// x1 wire 286
   632.40,		// x1 wire 287
   637.40,		// x1 wire 288
   642.40,		// x1 wire 289
   647.40,		// x1 wire 290
   652.40,		// x1 wire 291
   657.40,		// x1 wire 292
   662.40,		// x1 wire 293
   667.40,		// x1 wire 294
   672.40,		// x1 wire 295
   677.40,		// x1 wire 296
   682.40,		// x1 wire 297
   687.40,		// x1 wire 298
   692.40,		// x1 wire 299
   697.40,		// x1 wire 300
   702.40,		// x1 wire 301
   707.40,		// x1 wire 302
   712.40,		// x1 wire 303
   717.40,		// x1 wire 304
   722.40,		// x1 wire 305
   727.40,		// x1 wire 306
   732.40,		// x1 wire 307
   737.40,		// x1 wire 308
   742.40,		// x1 wire 309
   747.40,		// x1 wire 310
   752.40,		// x1 wire 311
   757.40,		// x1 wire 312
   762.40,		// x1 wire 313
   767.40,		// x1 wire 314
   772.40,		// x1 wire 315
   777.40,		// x1 wire 316
   782.40,		// x1 wire 317
   787.40,		// x1 wire 318
   792.40,		// x1 wire 319
   797.40,		// x1 wire 320
   802.40,		// x1 wire 321
   807.40,		// x1 wire 322
   812.40,		// x1 wire 323
   817.40,		// x1 wire 324
   822.40,		// x1 wire 325
   827.40,		// x1 wire 326
   832.40,		// x1 wire 327
   837.40,		// x1 wire 328
   842.40,		// x1 wire 329
   847.40,		// x1 wire 330
   852.40,		// x1 wire 331
   857.40,		// x1 wire 332
   862.40,		// x1 wire 333
   867.40,		// x1 wire 334
   872.40,		// x1 wire 335
   877.40,		// x1 wire 336
   882.40,		// x1 wire 337
   887.40,		// x1 wire 338
   892.40,		// x1 wire 339
   897.40,		// x1 wire 340
   902.40,		// x1 wire 341
   907.40,		// x1 wire 342
   912.40,		// x1 wire 343
   917.40,		// x1 wire 344
   922.40,		// x1 wire 345
   927.40,		// x1 wire 346
   932.40,		// x1 wire 347
   937.40,		// x1 wire 348
   942.40,		// x1 wire 349
   947.40,		// x1 wire 350
   952.40,		// x1 wire 351
   957.40,		// x1 wire 352
   962.40,		// x1 wire 353
   967.40,		// x1 wire 354
   972.40,		// x1 wire 355
   977.40,		// x1 wire 356
   982.40,		// x1 wire 357
   987.40,		// x1 wire 358
   992.40,		// x1 wire 359
   997.40,		// x1 wire 360
  1002.40,		// x1 wire 361
  1007.40,		// x1 wire 362
  1012.40,		// x1 wire 363
  1017.40,		// x1 wire 364
  1022.40,		// x1 wire 365
  1027.40,		// x1 wire 366
  1032.40,		// x1 wire 367
  1037.40,		// x1 wire 368
  1042.40,		// x1 wire 369
  1047.40,		// x1 wire 370
  1052.40,		// x1 wire 371
  1057.40,		// x1 wire 372
  1062.40,		// x1 wire 373
  1067.40,		// x1 wire 374
  1072.40,		// x1 wire 375
  1077.40,		// x1 wire 376
  1082.40,		// x1 wire 377
  1087.40,		// x1 wire 378
  1092.40,		// x1 wire 379
  1097.40,		// x1 wire 380
  1102.40,		// x1 wire 381
  1107.40,		// x1 wire 382
  1112.40,		// x1 wire 383
  1117.40,		// x1 wire 384
  1122.40,		// x1 wire 385
  1127.40,		// x1 wire 386
  1132.40,		// x1 wire 387
  1137.40,		// x1 wire 388
  1142.40,		// x1 wire 389
  1147.40,		// x1 wire 390
  1152.40,		// x1 wire 391
  1157.40,		// x1 wire 392
  1162.40,		// x1 wire 393
  1167.40,		// x1 wire 394
  1172.40,		// x1 wire 395
  1177.40,		// x1 wire 396
  1182.40,		// x1 wire 397
  1187.40,		// x1 wire 398
  1192.40,		// x1 wire 399
  1197.40,		// x1 wire 400
  1202.40,		// x1 wire 401
  1207.40,		// x1 wire 402
  1212.40,		// x1 wire 403
  1217.40,		// x1 wire 404
  1222.40,		// x1 wire 405
  1227.40,		// x1 wire 406
  1232.40,		// x1 wire 407
  1237.40,		// x1 wire 408
  1242.40,		// x1 wire 409
  1247.40,		// x1 wire 410
  1252.40,		// x1 wire 411
  1257.40,		// x1 wire 412
  1262.40,		// x1 wire 413
  1267.40,		// x1 wire 414
  1272.40,		// x1 wire 415
  1277.40,		// x1 wire 416
  1282.40,		// x1 wire 417
  1287.40,		// x1 wire 418
  1292.40,		// x1 wire 419
  1297.40,		// x1 wire 420
  1302.40,		// x1 wire 421
  1307.40,		// x1 wire 422
  1312.40,		// x1 wire 423
  1317.40,		// x1 wire 424
  1322.40,		// x1 wire 425
  1327.40,		// x1 wire 426
  1332.40,		// x1 wire 427
  1337.40,		// x1 wire 428
  1342.40,		// x1 wire 429
  1347.40,		// x1 wire 430
  1352.40,		// x1 wire 431
  1357.40,		// x1 wire 432
  1362.40,		// x1 wire 433
  1367.40,		// x1 wire 434
  1372.40,		// x1 wire 435
  1377.40,		// x1 wire 436
  1382.40,		// x1 wire 437
  1387.40,		// x1 wire 438
  1392.40,		// x1 wire 439
  1397.40,		// x1 wire 440
  1402.40,		// x1 wire 441
  1407.40,		// x1 wire 442
  1412.40,		// x1 wire 443
  1417.40,		// x1 wire 444
  1422.40,		// x1 wire 445
  1427.40,		// x1 wire 446
  1432.40,		// x1 wire 447
  1437.40		// x1 wire 448
};

static float
position_cs1[CH_C_MAX_S1] = {
  -727.92,		// s1 wire 1
  -722.92,		// s1 wire 2
  -717.92,		// s1 wire 3
  -712.92,		// s1 wire 4
  -707.92,		// s1 wire 5
  -702.92,		// s1 wire 6
  -697.92,		// s1 wire 7
  -692.92,		// s1 wire 8
  -687.92,		// s1 wire 9
  -682.92,		// s1 wire 10
  -677.92,		// s1 wire 11
  -672.92,		// s1 wire 12
  -667.92,		// s1 wire 13
  -662.92,		// s1 wire 14
  -657.92,		// s1 wire 15
  -652.92,		// s1 wire 16
  -647.92,		// s1 wire 17
  -642.92,		// s1 wire 18
  -637.92,		// s1 wire 19
  -632.92,		// s1 wire 20
  -627.92,		// s1 wire 21
  -622.92,		// s1 wire 22
  -617.92,		// s1 wire 23
  -612.92,		// s1 wire 24
  -607.92,		// s1 wire 25
  -602.92,		// s1 wire 26
  -597.92,		// s1 wire 27
  -592.92,		// s1 wire 28
  -587.92,		// s1 wire 29
  -582.92,		// s1 wire 30
  -577.92,		// s1 wire 31
  -572.92,		// s1 wire 32
  -567.92,		// s1 wire 33
  -562.92,		// s1 wire 34
  -557.92,		// s1 wire 35
  -552.92,		// s1 wire 36
  -547.92,		// s1 wire 37
  -542.92,		// s1 wire 38
  -537.92,		// s1 wire 39
  -532.92,		// s1 wire 40
  -527.92,		// s1 wire 41
  -522.92,		// s1 wire 42
  -517.92,		// s1 wire 43
  -512.92,		// s1 wire 44
  -507.92,		// s1 wire 45
  -502.92,		// s1 wire 46
  -497.92,		// s1 wire 47
  -492.92,		// s1 wire 48
  -487.92,		// s1 wire 49
  -482.92,		// s1 wire 50
  -477.92,		// s1 wire 51
  -472.92,		// s1 wire 52
  -467.92,		// s1 wire 53
  -462.92,		// s1 wire 54
  -457.92,		// s1 wire 55
  -452.92,		// s1 wire 56
  -447.92,		// s1 wire 57
  -442.92,		// s1 wire 58
  -437.92,		// s1 wire 59
  -432.92,		// s1 wire 60
  -427.92,		// s1 wire 61
  -422.92,		// s1 wire 62
  -417.92,		// s1 wire 63
  -412.92,		// s1 wire 64
  -407.92,		// s1 wire 65
  -402.92,		// s1 wire 66
  -397.92,		// s1 wire 67
  -392.92,		// s1 wire 68
  -387.92,		// s1 wire 69
  -382.92,		// s1 wire 70
  -377.92,		// s1 wire 71
  -372.92,		// s1 wire 72
  -367.92,		// s1 wire 73
  -362.92,		// s1 wire 74
  -357.92,		// s1 wire 75
  -352.92,		// s1 wire 76
  -347.92,		// s1 wire 77
  -342.92,		// s1 wire 78
  -337.92,		// s1 wire 79
  -332.92,		// s1 wire 80
  -327.92,		// s1 wire 81
  -322.92,		// s1 wire 82
  -317.92,		// s1 wire 83
  -312.92,		// s1 wire 84
  -307.92,		// s1 wire 85
  -302.92,		// s1 wire 86
  -297.92,		// s1 wire 87
  -292.92,		// s1 wire 88
  -287.92,		// s1 wire 89
  -282.92,		// s1 wire 90
  -277.92,		// s1 wire 91
  -272.92,		// s1 wire 92
  -267.92,		// s1 wire 93
  -262.92,		// s1 wire 94
  -257.92,		// s1 wire 95
  -252.92,		// s1 wire 96
  -247.92,		// s1 wire 97
  -242.92,		// s1 wire 98
  -237.92,		// s1 wire 99
  -232.92,		// s1 wire 100
  -227.92,		// s1 wire 101
  -222.92,		// s1 wire 102
  -217.92,		// s1 wire 103
  -212.92,		// s1 wire 104
  -207.92,		// s1 wire 105
  -202.92,		// s1 wire 106
  -197.92,		// s1 wire 107
  -192.92,		// s1 wire 108
  -187.92,		// s1 wire 109
  -182.92,		// s1 wire 110
  -177.92,		// s1 wire 111
  -172.92,		// s1 wire 112
  -167.92,		// s1 wire 113
  -162.92,		// s1 wire 114
  -157.92,		// s1 wire 115
  -152.92,		// s1 wire 116
  -147.92,		// s1 wire 117
  -142.92,		// s1 wire 118
  -137.92,		// s1 wire 119
  -132.92,		// s1 wire 120
  -127.92,		// s1 wire 121
  -122.92,		// s1 wire 122
  -117.92,		// s1 wire 123
  -112.92,		// s1 wire 124
  -107.92,		// s1 wire 125
  -102.92,		// s1 wire 126
   -97.92,		// s1 wire 127
   -92.92,		// s1 wire 128
   -87.92,		// s1 wire 129
   -82.92,		// s1 wire 130
   -77.92,		// s1 wire 131
   -72.92,		// s1 wire 132
   -67.92,		// s1 wire 133
   -62.92,		// s1 wire 134
   -57.92,		// s1 wire 135
   -52.92,		// s1 wire 136
   -47.92,		// s1 wire 137
   -42.92,		// s1 wire 138
   -37.92,		// s1 wire 139
   -32.92,		// s1 wire 140
   -27.92,		// s1 wire 141
   -22.92,		// s1 wire 142
   -17.92,		// s1 wire 143
   -12.92,		// s1 wire 144
    -7.92,		// s1 wire 145
    -2.92,		// s1 wire 146
     2.08,		// s1 wire 147
     7.08,		// s1 wire 148
    12.08,		// s1 wire 149
    17.08,		// s1 wire 150
    22.08,		// s1 wire 151
    27.08,		// s1 wire 152
    32.08,		// s1 wire 153
    37.08,		// s1 wire 154
    42.08,		// s1 wire 155
    47.08,		// s1 wire 156
    52.08,		// s1 wire 157
    57.08,		// s1 wire 158
    62.08,		// s1 wire 159
    67.08,		// s1 wire 160
    72.08,		// s1 wire 161
    77.08,		// s1 wire 162
    82.08,		// s1 wire 163
    87.08,		// s1 wire 164
    92.08,		// s1 wire 165
    97.08,		// s1 wire 166
   102.08,		// s1 wire 167
   107.08,		// s1 wire 168
   112.08,		// s1 wire 169
   117.08,		// s1 wire 170
   122.08,		// s1 wire 171
   127.08,		// s1 wire 172
   132.08,		// s1 wire 173
   137.08,		// s1 wire 174
   142.08,		// s1 wire 175
   147.08,		// s1 wire 176
   152.08,		// s1 wire 177
   157.08,		// s1 wire 178
   162.08,		// s1 wire 179
   167.08,		// s1 wire 180
   172.08,		// s1 wire 181
   177.08,		// s1 wire 182
   182.08,		// s1 wire 183
   187.08,		// s1 wire 184
   192.08,		// s1 wire 185
   197.08,		// s1 wire 186
   202.08,		// s1 wire 187
   207.08,		// s1 wire 188
   212.08,		// s1 wire 189
   217.08,		// s1 wire 190
   222.08,		// s1 wire 191
   227.08,		// s1 wire 192
   232.08,		// s1 wire 193
   237.08,		// s1 wire 194
   242.08,		// s1 wire 195
   247.08,		// s1 wire 196
   252.08,		// s1 wire 197
   257.08,		// s1 wire 198
   262.08,		// s1 wire 199
   267.08,		// s1 wire 200
   272.08,		// s1 wire 201
   277.08,		// s1 wire 202
   282.08,		// s1 wire 203
   287.08,		// s1 wire 204
   292.08,		// s1 wire 205
   297.08,		// s1 wire 206
   302.08,		// s1 wire 207
   307.08,		// s1 wire 208
   312.08,		// s1 wire 209
   317.08,		// s1 wire 210
   322.08,		// s1 wire 211
   327.08,		// s1 wire 212
   332.08,		// s1 wire 213
   337.08,		// s1 wire 214
   342.08,		// s1 wire 215
   347.08,		// s1 wire 216
   352.08,		// s1 wire 217
   357.08,		// s1 wire 218
   362.08,		// s1 wire 219
   367.08,		// s1 wire 220
   372.08,		// s1 wire 221
   377.08,		// s1 wire 222
   382.08,		// s1 wire 223
   387.08,		// s1 wire 224
   392.08,		// s1 wire 225
   397.08,		// s1 wire 226
   402.08,		// s1 wire 227
   407.08,		// s1 wire 228
   412.08,		// s1 wire 229
   417.08,		// s1 wire 230
   422.08,		// s1 wire 231
   427.08,		// s1 wire 232
   432.08,		// s1 wire 233
   437.08,		// s1 wire 234
   442.08,		// s1 wire 235
   447.08,		// s1 wire 236
   452.08,		// s1 wire 237
   457.08,		// s1 wire 238
   462.08,		// s1 wire 239
   467.08,		// s1 wire 240
   472.08,		// s1 wire 241
   477.08,		// s1 wire 242
   482.08,		// s1 wire 243
   487.08,		// s1 wire 244
   492.08,		// s1 wire 245
   497.08,		// s1 wire 246
   502.08,		// s1 wire 247
   507.08,		// s1 wire 248
   512.08,		// s1 wire 249
   517.08,		// s1 wire 250
   522.08,		// s1 wire 251
   527.08,		// s1 wire 252
   532.08,		// s1 wire 253
   537.08,		// s1 wire 254
   542.08,		// s1 wire 255
   547.08,		// s1 wire 256
   552.08,		// s1 wire 257
   557.08,		// s1 wire 258
   562.08,		// s1 wire 259
   567.08,		// s1 wire 260
   572.08,		// s1 wire 261
   577.08,		// s1 wire 262
   582.08,		// s1 wire 263
   587.08,		// s1 wire 264
   592.08,		// s1 wire 265
   597.08,		// s1 wire 266
   602.08,		// s1 wire 267
   607.08,		// s1 wire 268
   612.08,		// s1 wire 269
   617.08,		// s1 wire 270
   622.08,		// s1 wire 271
   627.08,		// s1 wire 272
   632.08,		// s1 wire 273
   637.08,		// s1 wire 274
   642.08,		// s1 wire 275
   647.08,		// s1 wire 276
   652.08,		// s1 wire 277
   657.08,		// s1 wire 278
   662.08,		// s1 wire 279
   667.08,		// s1 wire 280
   672.08,		// s1 wire 281
   677.08,		// s1 wire 282
   682.08,		// s1 wire 283
   687.08,		// s1 wire 284
   692.08,		// s1 wire 285
   697.08,		// s1 wire 286
   702.08,		// s1 wire 287
   707.08,		// s1 wire 288
   712.08,		// s1 wire 289
   717.08,		// s1 wire 290
   722.08,		// s1 wire 291
   727.08,		// s1 wire 292
   732.08,		// s1 wire 293
   737.08,		// s1 wire 294
   742.08,		// s1 wire 295
   747.08,		// s1 wire 296
   752.08,		// s1 wire 297
   757.08,		// s1 wire 298
   762.08,		// s1 wire 299
   767.08,		// s1 wire 300
   772.08,		// s1 wire 301
   777.08,		// s1 wire 302
   782.08,		// s1 wire 303
   787.08,		// s1 wire 304
   792.08,		// s1 wire 305
   797.08,		// s1 wire 306
   802.08,		// s1 wire 307
   807.08,		// s1 wire 308
   812.08,		// s1 wire 309
   817.08,		// s1 wire 310
   822.08,		// s1 wire 311
   827.08,		// s1 wire 312
   832.08,		// s1 wire 313
   837.08,		// s1 wire 314
   842.08,		// s1 wire 315
   847.08,		// s1 wire 316
   852.08,		// s1 wire 317
   857.08,		// s1 wire 318
   862.08,		// s1 wire 319
   867.08,		// s1 wire 320
   872.08,		// s1 wire 321
   877.08,		// s1 wire 322
   882.08,		// s1 wire 323
   887.08,		// s1 wire 324
   892.08,		// s1 wire 325
   897.08,		// s1 wire 326
   902.08,		// s1 wire 327
   907.08,		// s1 wire 328
   912.08,		// s1 wire 329
   917.08,		// s1 wire 330
   922.08,		// s1 wire 331
   927.08,		// s1 wire 332
   932.08,		// s1 wire 333
   937.08,		// s1 wire 334
   942.08,		// s1 wire 335
   947.08,		// s1 wire 336
   952.08,		// s1 wire 337
   957.08,		// s1 wire 338
   962.08,		// s1 wire 339
   967.08,		// s1 wire 340
   972.08,		// s1 wire 341
   977.08,		// s1 wire 342
   982.08,		// s1 wire 343
   987.08,		// s1 wire 344
   992.08,		// s1 wire 345
   997.08,		// s1 wire 346
  1002.08,		// s1 wire 347
  1007.08,		// s1 wire 348
  1012.08,		// s1 wire 349
  1017.08,		// s1 wire 350
  1022.08,		// s1 wire 351
  1027.08		// s1 wire 352
};

static float
position_cx2[CH_C_MAX_X2] = {
  -374.60,		// x2 wire 1
  -369.60,		// x2 wire 2
  -364.60,		// x2 wire 3
  -359.60,		// x2 wire 4
  -354.60,		// x2 wire 5
  -349.60,		// x2 wire 6
  -344.60,		// x2 wire 7
  -339.60,		// x2 wire 8
  -334.60,		// x2 wire 9
  -329.60,		// x2 wire 10
  -324.60,		// x2 wire 11
  -319.60,		// x2 wire 12
  -314.60,		// x2 wire 13
  -309.60,		// x2 wire 14
  -304.60,		// x2 wire 15
  -299.60,		// x2 wire 16
  -294.60,		// x2 wire 17
  -289.60,		// x2 wire 18
  -284.60,		// x2 wire 19
  -279.60,		// x2 wire 20
  -274.60,		// x2 wire 21
  -269.60,		// x2 wire 22
  -264.60,		// x2 wire 23
  -259.60,		// x2 wire 24
  -254.60,		// x2 wire 25
  -249.60,		// x2 wire 26
  -244.60,		// x2 wire 27
  -239.60,		// x2 wire 28
  -234.60,		// x2 wire 29
  -229.60,		// x2 wire 30
  -224.60,		// x2 wire 31
  -219.60,		// x2 wire 32
  -214.60,		// x2 wire 33
  -209.60,		// x2 wire 34
  -204.60,		// x2 wire 35
  -199.60,		// x2 wire 36
  -194.60,		// x2 wire 37
  -189.60,		// x2 wire 38
  -184.60,		// x2 wire 39
  -179.60,		// x2 wire 40
  -174.60,		// x2 wire 41
  -169.60,		// x2 wire 42
  -164.60,		// x2 wire 43
  -159.60,		// x2 wire 44
  -154.60,		// x2 wire 45
  -149.60,		// x2 wire 46
  -144.60,		// x2 wire 47
  -139.60,		// x2 wire 48
  -134.60,		// x2 wire 49
  -129.60,		// x2 wire 50
  -124.60,		// x2 wire 51
  -119.60,		// x2 wire 52
  -114.60,		// x2 wire 53
  -109.60,		// x2 wire 54
  -104.60,		// x2 wire 55
   -99.60,		// x2 wire 56
   -94.60,		// x2 wire 57
   -89.60,		// x2 wire 58
   -84.60,		// x2 wire 59
   -79.60,		// x2 wire 60
   -74.60,		// x2 wire 61
   -69.60,		// x2 wire 62
   -64.60,		// x2 wire 63
   -59.60,		// x2 wire 64
   -54.60,		// x2 wire 65
   -49.60,		// x2 wire 66
   -44.60,		// x2 wire 67
   -39.60,		// x2 wire 68
   -34.60,		// x2 wire 69
   -29.60,		// x2 wire 70
   -24.60,		// x2 wire 71
   -19.60,		// x2 wire 72
   -14.60,		// x2 wire 73
    -9.60,		// x2 wire 74
    -4.60,		// x2 wire 75
     0.40,		// x2 wire 76
     5.40,		// x2 wire 77
    10.40,		// x2 wire 78
    15.40,		// x2 wire 79
    20.40,		// x2 wire 80
    25.40,		// x2 wire 81
    30.40,		// x2 wire 82
    35.40,		// x2 wire 83
    40.40,		// x2 wire 84
    45.40,		// x2 wire 85
    50.40,		// x2 wire 86
    55.40,		// x2 wire 87
    60.40,		// x2 wire 88
    65.40,		// x2 wire 89
    70.40,		// x2 wire 90
    75.40,		// x2 wire 91
    80.40,		// x2 wire 92
    85.40,		// x2 wire 93
    90.40,		// x2 wire 94
    95.40,		// x2 wire 95
   100.40,		// x2 wire 96
   105.40,		// x2 wire 97
   110.40,		// x2 wire 98
   115.40,		// x2 wire 99
   120.40,		// x2 wire 100
   125.40,		// x2 wire 101
   130.40,		// x2 wire 102
   135.40,		// x2 wire 103
   140.40,		// x2 wire 104
   145.40,		// x2 wire 105
   150.40,		// x2 wire 106
   155.40,		// x2 wire 107
   160.40,		// x2 wire 108
   165.40,		// x2 wire 109
   170.40,		// x2 wire 110
   175.40,		// x2 wire 111
   180.40,		// x2 wire 112
   185.40,		// x2 wire 113
   190.40,		// x2 wire 114
   195.40,		// x2 wire 115
   200.40,		// x2 wire 116
   205.40,		// x2 wire 117
   210.40,		// x2 wire 118
   215.40,		// x2 wire 119
   220.40,		// x2 wire 120
   225.40,		// x2 wire 121
   230.40,		// x2 wire 122
   235.40,		// x2 wire 123
   240.40,		// x2 wire 124
   245.40,		// x2 wire 125
   250.40,		// x2 wire 126
   255.40,		// x2 wire 127
   260.40,		// x2 wire 128
   265.40,		// x2 wire 129
   270.40,		// x2 wire 130
   275.40,		// x2 wire 131
   280.40,		// x2 wire 132
   285.40,		// x2 wire 133
   290.40,		// x2 wire 134
   295.40,		// x2 wire 135
   300.40,		// x2 wire 136
   305.40,		// x2 wire 137
   310.40,		// x2 wire 138
   315.40,		// x2 wire 139
   320.40,		// x2 wire 140
   325.40,		// x2 wire 141
   330.40,		// x2 wire 142
   335.40,		// x2 wire 143
   340.40,		// x2 wire 144
   345.40,		// x2 wire 145
   350.40,		// x2 wire 146
   355.40,		// x2 wire 147
   360.40,		// x2 wire 148
   365.40,		// x2 wire 149
   370.40,		// x2 wire 150
   375.40,		// x2 wire 151
   380.40,		// x2 wire 152
   385.40,		// x2 wire 153
   390.40,		// x2 wire 154
   395.40,		// x2 wire 155
   400.40,		// x2 wire 156
   405.40,		// x2 wire 157
   410.40,		// x2 wire 158
   415.40,		// x2 wire 159
   420.40,		// x2 wire 160
   425.40,		// x2 wire 161
   430.40,		// x2 wire 162
   435.40,		// x2 wire 163
   440.40,		// x2 wire 164
   445.40,		// x2 wire 165
   450.40,		// x2 wire 166
   455.40,		// x2 wire 167
   460.40,		// x2 wire 168
   465.40,		// x2 wire 169
   470.40,		// x2 wire 170
   475.40,		// x2 wire 171
   480.40,		// x2 wire 172
   485.40,		// x2 wire 173
   490.40,		// x2 wire 174
   495.40,		// x2 wire 175
   500.40,		// x2 wire 176
   505.40,		// x2 wire 177
   510.40,		// x2 wire 178
   515.40,		// x2 wire 179
   520.40,		// x2 wire 180
   525.40,		// x2 wire 181
   530.40,		// x2 wire 182
   535.40,		// x2 wire 183
   540.40,		// x2 wire 184
   545.40,		// x2 wire 185
   550.40,		// x2 wire 186
   555.40,		// x2 wire 187
   560.40,		// x2 wire 188
   565.40,		// x2 wire 189
   570.40,		// x2 wire 190
   575.40,		// x2 wire 191
   580.40,		// x2 wire 192
   585.40,		// x2 wire 193
   590.40,		// x2 wire 194
   595.40,		// x2 wire 195
   600.40,		// x2 wire 196
   605.40,		// x2 wire 197
   610.40,		// x2 wire 198
   615.40,		// x2 wire 199
   620.40,		// x2 wire 200
   625.40,		// x2 wire 201
   630.40,		// x2 wire 202
   635.40,		// x2 wire 203
   640.40,		// x2 wire 204
   645.40,		// x2 wire 205
   650.40,		// x2 wire 206
   655.40,		// x2 wire 207
   660.40,		// x2 wire 208
   665.40,		// x2 wire 209
   670.40,		// x2 wire 210
   675.40,		// x2 wire 211
   680.40,		// x2 wire 212
   685.40,		// x2 wire 213
   690.40,		// x2 wire 214
   695.40,		// x2 wire 215
   700.40,		// x2 wire 216
   705.40,		// x2 wire 217
   710.40,		// x2 wire 218
   715.40,		// x2 wire 219
   720.40,		// x2 wire 220
   725.40,		// x2 wire 221
   730.40,		// x2 wire 222
   735.40,		// x2 wire 223
   740.40,		// x2 wire 224
   745.40,		// x2 wire 225
   750.40,		// x2 wire 226
   755.40,		// x2 wire 227
   760.40,		// x2 wire 228
   765.40,		// x2 wire 229
   770.40,		// x2 wire 230
   775.40,		// x2 wire 231
   780.40,		// x2 wire 232
   785.40,		// x2 wire 233
   790.40,		// x2 wire 234
   795.40,		// x2 wire 235
   800.40,		// x2 wire 236
   805.40,		// x2 wire 237
   810.40,		// x2 wire 238
   815.40,		// x2 wire 239
   820.40,		// x2 wire 240
   825.40,		// x2 wire 241
   830.40,		// x2 wire 242
   835.40,		// x2 wire 243
   840.40,		// x2 wire 244
   845.40,		// x2 wire 245
   850.40,		// x2 wire 246
   855.40,		// x2 wire 247
   860.40,		// x2 wire 248
   865.40,		// x2 wire 249
   870.40,		// x2 wire 250
   875.40,		// x2 wire 251
   880.40,		// x2 wire 252
   885.40,		// x2 wire 253
   890.40,		// x2 wire 254
   895.40,		// x2 wire 255
   900.40,		// x2 wire 256
   905.40,		// x2 wire 257
   910.40,		// x2 wire 258
   915.40,		// x2 wire 259
   920.40,		// x2 wire 260
   925.40,		// x2 wire 261
   930.40,		// x2 wire 262
   935.40,		// x2 wire 263
   940.40,		// x2 wire 264
   945.40,		// x2 wire 265
   950.40,		// x2 wire 266
   955.40,		// x2 wire 267
   960.40,		// x2 wire 268
   965.40,		// x2 wire 269
   970.40,		// x2 wire 270
   975.40,		// x2 wire 271
   980.40,		// x2 wire 272
   985.40,		// x2 wire 273
   990.40,		// x2 wire 274
   995.40,		// x2 wire 275
  1000.40,		// x2 wire 276
  1005.40,		// x2 wire 277
  1010.40,		// x2 wire 278
  1015.40,		// x2 wire 279
  1020.40,		// x2 wire 280
  1025.40,		// x2 wire 281
  1030.40,		// x2 wire 282
  1035.40,		// x2 wire 283
  1040.40,		// x2 wire 284
  1045.40,		// x2 wire 285
  1050.40,		// x2 wire 286
  1055.40,		// x2 wire 287
  1060.40,		// x2 wire 288
  1065.40,		// x2 wire 289
  1070.40,		// x2 wire 290
  1075.40,		// x2 wire 291
  1080.40,		// x2 wire 292
  1085.40,		// x2 wire 293
  1090.40,		// x2 wire 294
  1095.40,		// x2 wire 295
  1100.40,		// x2 wire 296
  1105.40,		// x2 wire 297
  1110.40,		// x2 wire 298
  1115.40,		// x2 wire 299
  1120.40,		// x2 wire 300
  1125.40,		// x2 wire 301
  1130.40,		// x2 wire 302
  1135.40,		// x2 wire 303
  1140.40,		// x2 wire 304
  1145.40,		// x2 wire 305
  1150.40,		// x2 wire 306
  1155.40,		// x2 wire 307
  1160.40,		// x2 wire 308
  1165.40,		// x2 wire 309
  1170.40,		// x2 wire 310
  1175.40,		// x2 wire 311
  1180.40,		// x2 wire 312
  1185.40,		// x2 wire 313
  1190.40,		// x2 wire 314
  1195.40,		// x2 wire 315
  1200.40,		// x2 wire 316
  1205.40,		// x2 wire 317
  1210.40,		// x2 wire 318
  1215.40,		// x2 wire 319
  1220.40,		// x2 wire 320
  1225.40,		// x2 wire 321
  1230.40,		// x2 wire 322
  1235.40,		// x2 wire 323
  1240.40,		// x2 wire 324
  1245.40,		// x2 wire 325
  1250.40,		// x2 wire 326
  1255.40,		// x2 wire 327
  1260.40,		// x2 wire 328
  1265.40,		// x2 wire 329
  1270.40,		// x2 wire 330
  1275.40,		// x2 wire 331
  1280.40,		// x2 wire 332
  1285.40,		// x2 wire 333
  1290.40,		// x2 wire 334
  1295.40,		// x2 wire 335
  1300.40,		// x2 wire 336
  1305.40,		// x2 wire 337
  1310.40,		// x2 wire 338
  1315.40,		// x2 wire 339
  1320.40,		// x2 wire 340
  1325.40,		// x2 wire 341
  1330.40,		// x2 wire 342
  1335.40,		// x2 wire 343
  1340.40,		// x2 wire 344
  1345.40,		// x2 wire 345
  1350.40,		// x2 wire 346
  1355.40,		// x2 wire 347
  1360.40,		// x2 wire 348
  1365.40,		// x2 wire 349
  1370.40,		// x2 wire 350
  1375.40,		// x2 wire 351
  1380.40,		// x2 wire 352
  1385.40,		// x2 wire 353
  1390.40,		// x2 wire 354
  1395.40,		// x2 wire 355
  1400.40,		// x2 wire 356
  1405.40,		// x2 wire 357
  1410.40,		// x2 wire 358
  1415.40,		// x2 wire 359
  1420.40,		// x2 wire 360
  1425.40,		// x2 wire 361
  1430.40,		// x2 wire 362
  1435.40,		// x2 wire 363
  1440.40,		// x2 wire 364
  1445.40,		// x2 wire 365
  1450.40,		// x2 wire 366
  1455.40,		// x2 wire 367
  1460.40,		// x2 wire 368
  1465.40,		// x2 wire 369
  1470.40,		// x2 wire 370
  1475.40,		// x2 wire 371
  1480.40,		// x2 wire 372
  1485.40,		// x2 wire 373
  1490.40,		// x2 wire 374
  1495.40,		// x2 wire 375
  1500.40,		// x2 wire 376
  1505.40,		// x2 wire 377
  1510.40,		// x2 wire 378
  1515.40,		// x2 wire 379
  1520.40,		// x2 wire 380
  1525.40,		// x2 wire 381
  1530.40,		// x2 wire 382
  1535.40,		// x2 wire 383
  1540.40,		// x2 wire 384
  1545.40,		// x2 wire 385
  1550.40,		// x2 wire 386
  1555.40,		// x2 wire 387
  1560.40,		// x2 wire 388
  1565.40,		// x2 wire 389
  1570.40,		// x2 wire 390
  1575.40,		// x2 wire 391
  1580.40,		// x2 wire 392
  1585.40,		// x2 wire 393
  1590.40,		// x2 wire 394
  1595.40,		// x2 wire 395
  1600.40,		// x2 wire 396
  1605.40,		// x2 wire 397
  1610.40,		// x2 wire 398
  1615.40,		// x2 wire 399
  1620.40,		// x2 wire 400
  1625.40,		// x2 wire 401
  1630.40,		// x2 wire 402
  1635.40,		// x2 wire 403
  1640.40,		// x2 wire 404
  1645.40,		// x2 wire 405
  1650.40,		// x2 wire 406
  1655.40,		// x2 wire 407
  1660.40,		// x2 wire 408
  1665.40,		// x2 wire 409
  1670.40,		// x2 wire 410
  1675.40,		// x2 wire 411
  1680.40,		// x2 wire 412
  1685.40,		// x2 wire 413
  1690.40,		// x2 wire 414
  1695.40,		// x2 wire 415
  1700.40,		// x2 wire 416
  1705.40,		// x2 wire 417
  1710.40,		// x2 wire 418
  1715.40,		// x2 wire 419
  1720.40,		// x2 wire 420
  1725.40,		// x2 wire 421
  1730.40,		// x2 wire 422
  1735.40,		// x2 wire 423
  1740.40,		// x2 wire 424
  1745.40,		// x2 wire 425
  1750.40,		// x2 wire 426
  1755.40,		// x2 wire 427
  1760.40,		// x2 wire 428
  1765.40,		// x2 wire 429
  1770.40,		// x2 wire 430
  1775.40,		// x2 wire 431
  1780.40,		// x2 wire 432
  1785.40,		// x2 wire 433
  1790.40,		// x2 wire 434
  1795.40,		// x2 wire 435
  1800.40,		// x2 wire 436
  1805.40,		// x2 wire 437
  1810.40,		// x2 wire 438
  1815.40,		// x2 wire 439
  1820.40,		// x2 wire 440
  1825.40,		// x2 wire 441
  1830.40,		// x2 wire 442
  1835.40,		// x2 wire 443
  1840.40,		// x2 wire 444
  1845.40,		// x2 wire 445
  1850.40,		// x2 wire 446
  1855.40,		// x2 wire 447
  1860.40		// x2 wire 448
};

static float
position_cs2[CH_C_MAX_S2] = {
  -405.58,		// s2 wire 1
  -400.58,		// s2 wire 2
  -395.58,		// s2 wire 3
  -390.58,		// s2 wire 4
  -385.58,		// s2 wire 5
  -380.58,		// s2 wire 6
  -375.58,		// s2 wire 7
  -370.58,		// s2 wire 8
  -365.58,		// s2 wire 9
  -360.58,		// s2 wire 10
  -355.58,		// s2 wire 11
  -350.58,		// s2 wire 12
  -345.58,		// s2 wire 13
  -340.58,		// s2 wire 14
  -335.58,		// s2 wire 15
  -330.58,		// s2 wire 16
  -325.58,		// s2 wire 17
  -320.58,		// s2 wire 18
  -315.58,		// s2 wire 19
  -310.58,		// s2 wire 20
  -305.58,		// s2 wire 21
  -300.58,		// s2 wire 22
  -295.58,		// s2 wire 23
  -290.58,		// s2 wire 24
  -285.58,		// s2 wire 25
  -280.58,		// s2 wire 26
  -275.58,		// s2 wire 27
  -270.58,		// s2 wire 28
  -265.58,		// s2 wire 29
  -260.58,		// s2 wire 30
  -255.58,		// s2 wire 31
  -250.58,		// s2 wire 32
  -245.58,		// s2 wire 33
  -240.58,		// s2 wire 34
  -235.58,		// s2 wire 35
  -230.58,		// s2 wire 36
  -225.58,		// s2 wire 37
  -220.58,		// s2 wire 38
  -215.58,		// s2 wire 39
  -210.58,		// s2 wire 40
  -205.58,		// s2 wire 41
  -200.58,		// s2 wire 42
  -195.58,		// s2 wire 43
  -190.58,		// s2 wire 44
  -185.58,		// s2 wire 45
  -180.58,		// s2 wire 46
  -175.58,		// s2 wire 47
  -170.58,		// s2 wire 48
  -165.58,		// s2 wire 49
  -160.58,		// s2 wire 50
  -155.58,		// s2 wire 51
  -150.58,		// s2 wire 52
  -145.58,		// s2 wire 53
  -140.58,		// s2 wire 54
  -135.58,		// s2 wire 55
  -130.58,		// s2 wire 56
  -125.58,		// s2 wire 57
  -120.58,		// s2 wire 58
  -115.58,		// s2 wire 59
  -110.58,		// s2 wire 60
  -105.58,		// s2 wire 61
  -100.58,		// s2 wire 62
   -95.58,		// s2 wire 63
   -90.58,		// s2 wire 64
   -85.58,		// s2 wire 65
   -80.58,		// s2 wire 66
   -75.58,		// s2 wire 67
   -70.58,		// s2 wire 68
   -65.58,		// s2 wire 69
   -60.58,		// s2 wire 70
   -55.58,		// s2 wire 71
   -50.58,		// s2 wire 72
   -45.58,		// s2 wire 73
   -40.58,		// s2 wire 74
   -35.58,		// s2 wire 75
   -30.58,		// s2 wire 76
   -25.58,		// s2 wire 77
   -20.58,		// s2 wire 78
   -15.58,		// s2 wire 79
   -10.58,		// s2 wire 80
    -5.58,		// s2 wire 81
    -0.58,		// s2 wire 82
     4.42,		// s2 wire 83
     9.42,		// s2 wire 84
    14.42,		// s2 wire 85
    19.42,		// s2 wire 86
    24.42,		// s2 wire 87
    29.42,		// s2 wire 88
    34.42,		// s2 wire 89
    39.42,		// s2 wire 90
    44.42,		// s2 wire 91
    49.42,		// s2 wire 92
    54.42,		// s2 wire 93
    59.42,		// s2 wire 94
    64.42,		// s2 wire 95
    69.42,		// s2 wire 96
    74.42,		// s2 wire 97
    79.42,		// s2 wire 98
    84.42,		// s2 wire 99
    89.42,		// s2 wire 100
    94.42,		// s2 wire 101
    99.42,		// s2 wire 102
   104.42,		// s2 wire 103
   109.42,		// s2 wire 104
   114.42,		// s2 wire 105
   119.42,		// s2 wire 106
   124.42,		// s2 wire 107
   129.42,		// s2 wire 108
   134.42,		// s2 wire 109
   139.42,		// s2 wire 110
   144.42,		// s2 wire 111
   149.42,		// s2 wire 112
   154.42,		// s2 wire 113
   159.42,		// s2 wire 114
   164.42,		// s2 wire 115
   169.42,		// s2 wire 116
   174.42,		// s2 wire 117
   179.42,		// s2 wire 118
   184.42,		// s2 wire 119
   189.42,		// s2 wire 120
   194.42,		// s2 wire 121
   199.42,		// s2 wire 122
   204.42,		// s2 wire 123
   209.42,		// s2 wire 124
   214.42,		// s2 wire 125
   219.42,		// s2 wire 126
   224.42,		// s2 wire 127
   229.42,		// s2 wire 128
   234.42,		// s2 wire 129
   239.42,		// s2 wire 130
   244.42,		// s2 wire 131
   249.42,		// s2 wire 132
   254.42,		// s2 wire 133
   259.42,		// s2 wire 134
   264.42,		// s2 wire 135
   269.42,		// s2 wire 136
   274.42,		// s2 wire 137
   279.42,		// s2 wire 138
   284.42,		// s2 wire 139
   289.42,		// s2 wire 140
   294.42,		// s2 wire 141
   299.42,		// s2 wire 142
   304.42,		// s2 wire 143
   309.42,		// s2 wire 144
   314.42,		// s2 wire 145
   319.42,		// s2 wire 146
   324.42,		// s2 wire 147
   329.42,		// s2 wire 148
   334.42,		// s2 wire 149
   339.42,		// s2 wire 150
   344.42,		// s2 wire 151
   349.42,		// s2 wire 152
   354.42,		// s2 wire 153
   359.42,		// s2 wire 154
   364.42,		// s2 wire 155
   369.42,		// s2 wire 156
   374.42,		// s2 wire 157
   379.42,		// s2 wire 158
   384.42,		// s2 wire 159
   389.42,		// s2 wire 160
   394.42,		// s2 wire 161
   399.42,		// s2 wire 162
   404.42,		// s2 wire 163
   409.42,		// s2 wire 164
   414.42,		// s2 wire 165
   419.42,		// s2 wire 166
   424.42,		// s2 wire 167
   429.42,		// s2 wire 168
   434.42,		// s2 wire 169
   439.42,		// s2 wire 170
   444.42,		// s2 wire 171
   449.42,		// s2 wire 172
   454.42,		// s2 wire 173
   459.42,		// s2 wire 174
   464.42,		// s2 wire 175
   469.42,		// s2 wire 176
   474.42,		// s2 wire 177
   479.42,		// s2 wire 178
   484.42,		// s2 wire 179
   489.42,		// s2 wire 180
   494.42,		// s2 wire 181
   499.42,		// s2 wire 182
   504.42,		// s2 wire 183
   509.42,		// s2 wire 184
   514.42,		// s2 wire 185
   519.42,		// s2 wire 186
   524.42,		// s2 wire 187
   529.42,		// s2 wire 188
   534.42,		// s2 wire 189
   539.42,		// s2 wire 190
   544.42,		// s2 wire 191
   549.42,		// s2 wire 192
   554.42,		// s2 wire 193
   559.42,		// s2 wire 194
   564.42,		// s2 wire 195
   569.42,		// s2 wire 196
   574.42,		// s2 wire 197
   579.42,		// s2 wire 198
   584.42,		// s2 wire 199
   589.42,		// s2 wire 200
   594.42,		// s2 wire 201
   599.42,		// s2 wire 202
   604.42,		// s2 wire 203
   609.42,		// s2 wire 204
   614.42,		// s2 wire 205
   619.42,		// s2 wire 206
   624.42,		// s2 wire 207
   629.42,		// s2 wire 208
   634.42,		// s2 wire 209
   639.42,		// s2 wire 210
   644.42,		// s2 wire 211
   649.42,		// s2 wire 212
   654.42,		// s2 wire 213
   659.42,		// s2 wire 214
   664.42,		// s2 wire 215
   669.42,		// s2 wire 216
   674.42,		// s2 wire 217
   679.42,		// s2 wire 218
   684.42,		// s2 wire 219
   689.42,		// s2 wire 220
   694.42,		// s2 wire 221
   699.42,		// s2 wire 222
   704.42,		// s2 wire 223
   709.42,		// s2 wire 224
   714.42,		// s2 wire 225
   719.42,		// s2 wire 226
   724.42,		// s2 wire 227
   729.42,		// s2 wire 228
   734.42,		// s2 wire 229
   739.42,		// s2 wire 230
   744.42,		// s2 wire 231
   749.42,		// s2 wire 232
   754.42,		// s2 wire 233
   759.42,		// s2 wire 234
   764.42,		// s2 wire 235
   769.42,		// s2 wire 236
   774.42,		// s2 wire 237
   779.42,		// s2 wire 238
   784.42,		// s2 wire 239
   789.42,		// s2 wire 240
   794.42,		// s2 wire 241
   799.42,		// s2 wire 242
   804.42,		// s2 wire 243
   809.42,		// s2 wire 244
   814.42,		// s2 wire 245
   819.42,		// s2 wire 246
   824.42,		// s2 wire 247
   829.42,		// s2 wire 248
   834.42,		// s2 wire 249
   839.42,		// s2 wire 250
   844.42,		// s2 wire 251
   849.42,		// s2 wire 252
   854.42,		// s2 wire 253
   859.42,		// s2 wire 254
   864.42,		// s2 wire 255
   869.42,		// s2 wire 256
   874.42,		// s2 wire 257
   879.42,		// s2 wire 258
   884.42,		// s2 wire 259
   889.42,		// s2 wire 260
   894.42,		// s2 wire 261
   899.42,		// s2 wire 262
   904.42,		// s2 wire 263
   909.42,		// s2 wire 264
   914.42,		// s2 wire 265
   919.42,		// s2 wire 266
   924.42,		// s2 wire 267
   929.42,		// s2 wire 268
   934.42,		// s2 wire 269
   939.42,		// s2 wire 270
   944.42,		// s2 wire 271
   949.42,		// s2 wire 272
   954.42,		// s2 wire 273
   959.42,		// s2 wire 274
   964.42,		// s2 wire 275
   969.42,		// s2 wire 276
   974.42,		// s2 wire 277
   979.42,		// s2 wire 278
   984.42,		// s2 wire 279
   989.42,		// s2 wire 280
   994.42,		// s2 wire 281
   999.42,		// s2 wire 282
  1004.42,		// s2 wire 283
  1009.42,		// s2 wire 284
  1014.42,		// s2 wire 285
  1019.42,		// s2 wire 286
  1024.42,		// s2 wire 287
  1029.42,		// s2 wire 288
  1034.42,		// s2 wire 289
  1039.42,		// s2 wire 290
  1044.42,		// s2 wire 291
  1049.42,		// s2 wire 292
  1054.42,		// s2 wire 293
  1059.42,		// s2 wire 294
  1064.42,		// s2 wire 295
  1069.42,		// s2 wire 296
  1074.42,		// s2 wire 297
  1079.42,		// s2 wire 298
  1084.42,		// s2 wire 299
  1089.42,		// s2 wire 300
  1094.42,		// s2 wire 301
  1099.42,		// s2 wire 302
  1104.42,		// s2 wire 303
  1109.42,		// s2 wire 304
  1114.42,		// s2 wire 305
  1119.42,		// s2 wire 306
  1124.42,		// s2 wire 307
  1129.42,		// s2 wire 308
  1134.42,		// s2 wire 309
  1139.42,		// s2 wire 310
  1144.42,		// s2 wire 311
  1149.42,		// s2 wire 312
  1154.42,		// s2 wire 313
  1159.42,		// s2 wire 314
  1164.42,		// s2 wire 315
  1169.42,		// s2 wire 316
  1174.42,		// s2 wire 317
  1179.42,		// s2 wire 318
  1184.42,		// s2 wire 319
  1189.42,		// s2 wire 320
  1194.42,		// s2 wire 321
  1199.42,		// s2 wire 322
  1204.42,		// s2 wire 323
  1209.42,		// s2 wire 324
  1214.42,		// s2 wire 325
  1219.42,		// s2 wire 326
  1224.42,		// s2 wire 327
  1229.42,		// s2 wire 328
  1234.42,		// s2 wire 329
  1239.42,		// s2 wire 330
  1244.42,		// s2 wire 331
  1249.42,		// s2 wire 332
  1254.42,		// s2 wire 333
  1259.42,		// s2 wire 334
  1264.42,		// s2 wire 335
  1269.42,		// s2 wire 336
  1274.42,		// s2 wire 337
  1279.42,		// s2 wire 338
  1284.42,		// s2 wire 339
  1289.42,		// s2 wire 340
  1294.42,		// s2 wire 341
  1299.42,		// s2 wire 342
  1304.42,		// s2 wire 343
  1309.42,		// s2 wire 344
  1314.42,		// s2 wire 345
  1319.42,		// s2 wire 346
  1324.42,		// s2 wire 347
  1329.42,		// s2 wire 348
  1334.42,		// s2 wire 349
  1339.42,		// s2 wire 350
  1344.42,		// s2 wire 351
  1349.42		// s2 wire 352
};

vdcPosOffs vdcPosOffsC = {
  { position_cx1, offset_cx1, CH_C_MAX_X1 },
  { position_cs1, offset_cs1, CH_C_MAX_S1 },
  { position_cx2, offset_cx2, CH_C_MAX_X2 },
  { position_cs2, offset_cs2, CH_C_MAX_S2 }
};

#endif
