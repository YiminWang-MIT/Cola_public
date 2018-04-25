//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/Digitizer.h,v 1.1 1999-02-03 23:00:43 distler Exp $
//
// Classes to decode digitizer information
//

#ifndef __DIGITIZER_H__
#define __DIGITIZER_H__

/**
 *
 */
class DigitBase {
 protected:
  unsigned int model, channels;
  unsigned int dataMask, wdcntMask;
  unsigned int chanMask, chanShift;
  unsigned int  optMask,  optShift;
 public:
  DigitBase(unsigned int d, unsigned int c, unsigned int o, unsigned int w);
  virtual ~DigitBase() { ; }
  virtual int print(unsigned int);
  virtual int handle(unsigned int);
};

/**
 * 
 */
class Digit1877 : public DigitBase {
 public:
  Digit1877() : DigitBase(0xFFFF, 0xFE0000, 0x10000, 0x7FF)
    { model = 1877; channels = 95; }
  virtual ~Digit1877() { ; }
};

/**
 * 
 */
class Digit1875 : public DigitBase {
 public:
  Digit1875() : DigitBase(0xFFF, 0x7F0000, 0x800000, 0x0)
    { model = 1875; channels = 63; }
  virtual ~Digit1875() { ; }
};

/**
 * 
 */
class Digit1881 : public DigitBase {
 public:
  Digit1881() : DigitBase(0x3FFF, 0x7E0000, 0x3000000, 0x7F)
    { model = 1881; channels = 63; }
  virtual ~Digit1881() { ; }
};

/**
 * 
 */
class Digit1151 : public DigitBase {
 public:
  Digit1151() : DigitBase(0xFFFFFFFF, 0x0, 0x0, 0xFF)
    { model = 1151; channels = 15; }
  virtual ~Digit1151() { ; }
};

/**
 * 
 */
class Digit1882 : public DigitBase {
 public:
  Digit1882() : DigitBase(0xFFF, 0x7F0000, 0x800000, 0x0)
    { model = 1882; channels = 95; }
  virtual ~Digit1882() { ; }
};

/**
 * 
 */
class Digit1876 : public DigitBase {
 public:
  Digit1876() : DigitBase(0xFFFF, 0xFE0000, 0x10000, 0x7FF)
    { model = 1876; channels = 95; }
  virtual ~Digit1876() { ; }
};

/**
 * 
 */
class Digit3123 : public DigitBase {
 public:
  Digit3123() : DigitBase(0x0, 0x0, 0x0, 0x0)
    { model = 3123; channels = 16; }
  virtual ~Digit3123() { ; }
};

/**
 * 
 */
class Digit1182 : public DigitBase {
 public:
  Digit1182() : DigitBase(0x0, 0x0, 0x0, 0x0)
    { model = 1182; channels = 8; }
  virtual ~Digit1182() { ; }
};

#endif /* __DIGITIZER_H__ */
