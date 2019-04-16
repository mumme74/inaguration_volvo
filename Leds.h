/*
 * Leds.h
 *
 *  Created on: 15 apr 2019
 *      Author: jof
 */

#include <stdint.h>
#include <FastLED.h>
#include <DList.h>

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


/**
 * @brief: a segment is built by 1 or more SegmentParts
 * combines 2 separate led strips with separate data out
 */
class Segment {
  DListDynamic<SegmentPart*> m_segmentParts;
  bool m_halted;
public:
  Segment();
  ~Segment();
  // add a subsegment to this segment, built up of several SegmentParts
  void addSegmentPart(SegmentPart *part);
  // how many SegmentParts does this segment have
  SegmentPart* segmentPart(size_t idx);
  void removeSegmentPart(size_t idx);
  size_t segmentSize() const;
  // halted status
  bool halted() const { return m_halted; }
  void setHalted(bool halt);

  // LEDs
  CRGB *operator [] (uint16_t idx) const;
  uint16_t size() const;


};

/**
 * brief: make several segments work combined
 * ie like different segments of a logo
 */
class SegmentCompound {
  DListDynamic<Segment*> m_segments;
public:
  SegmentCompound();
  ~SegmentCompound();

  /// add segment to to this compound
  void addSegment(Segment *segment);
  size_t segmentSize() const;
  void removeSegment(size_t idx);
  Segment* operator[] (size_t idx);
};

#endif /* LEDS_H_ */
