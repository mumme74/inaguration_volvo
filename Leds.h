/*
 * Leds.h
 *
 *  Created on: 15 apr 2019
 *      Author: jof
 */

#include <stdint.h>
#include <FastLED.h>
#include <DList.h>
#include "Actions.h"

#ifndef LEDS_H_
#define LEDS_H_

/**
 * @breif: a segment of leds within a single LED data out
 */
class SegmentPart {
  uint16_t m_firstIdx,
           m_lastIdx;
  CRGB **m_leds;

public:
  SegmentPart(CRGB **leds, uint16_t firstLed, uint16_t lastLed);
  ~SegmentPart();

  uint16_t firstLedIdx() const { return m_firstIdx; }
  uint16_t lastLedIdx() const { return m_lastIdx; }
  uint16_t size() const { return m_lastIdx - m_firstIdx; }
  CRGB *operator [] (uint16_t idx) const { return m_leds[idx]; }
};

//--------------------------------------------------------------

/**
 * @breif: Abstract base for all segments
 */
class SegmentCommon : public ActionsContainer {
protected:
  bool m_halted;
public:
  explicit SegmentCommon();
  virtual ~SegmentCommon();

  // halted status
  bool halted() const;
  void setHalted(bool halt);

  // tick, must be called from loop in root *.ino file
  void loop();

  // LEDs
  virtual CRGB* operator [] (uint16_t idx) = 0;
  virtual uint16_t size() const = 0;
};

// ---------------------------------------------------------

/**
 * @brief: a segment is built by 1 or more SegmentParts
 * combines 2 separate led strips with separate data out
 */
class Segment : public SegmentCommon {
  DListDynamic<SegmentPart*> m_segmentParts;
public:
  Segment();
  ~Segment();

  // add a subsegment to this segment, built up of several SegmentParts
  void addSegmentPart(SegmentPart *part);
  // how many SegmentParts does this segment have
  SegmentPart* segmentPart(size_t idx);
  void removeSegmentPart(size_t idx);
  size_t segmentSize() const;

  // LEDs
  CRGB *operator [] (uint16_t idx);
  uint16_t size() const;
};

// -----------------------------------------------------------

/**
 * brief: make several segments work combined
 * ie like different segments of a logo
 */
class SegmentCompound : public SegmentCommon {
  DListDynamic<Segment*> m_segments;
public:
  SegmentCompound();
  ~SegmentCompound();

  /// add segment to to this compound
  void addSegment(Segment *segment);
  size_t segmentSize() const;
  void removeSegment(size_t idx);
  Segment* segmentAt(size_t idx);

  // LEDs
  CRGB *operator [] (uint16_t idx);
  uint16_t size() const;
};

#endif /* LEDS_H_ */
