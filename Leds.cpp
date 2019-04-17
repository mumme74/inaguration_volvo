/*
 * Leds.cpp
 *
 *  Created on: 15 apr 2019
 *      Author: jof
 */

#include "Leds.h"

SegmentPart::SegmentPart(CRGB **leds,
                         uint16_t firstLed,
                         uint16_t lastLed) :
    m_firstIdx(firstLed), m_lastIdx(lastLed),
    m_leds(leds)
{
}

SegmentPart::~SegmentPart()
{
}

// --------------------------------------------------------------------

SegmentCommon::SegmentCommon() :
    ActionsContainer(),
    m_halted(true)
{
}

SegmentCommon::~SegmentCommon()
{
}

bool SegmentCommon::halted() const
{
  return m_halted;
}

void SegmentCommon::setHalted(bool halt)
{
  m_halted = halt;
}

void SegmentCommon::loop()
{
  if (!m_halted && m_actions.length()) {
    ActionBase *action = m_actions[m_currentIdx];
    action->loop();
  }
}

// --------------------------------------------------------------------

Segment::Segment() :
    SegmentCommon()
{
}

Segment::~Segment()
{
}

void Segment::addSegmentPart(SegmentPart *part)
{
  m_segmentParts.push(part);
}

SegmentPart* Segment::segmentPart(size_t idx)
{
  if (m_segmentParts.length() <= idx)
    return nullptr;
  return m_segmentParts[idx];
}

void Segment::removeSegmentPart(size_t idx)
{
  m_segmentParts.remove(idx);
}

size_t Segment::segmentSize() const
{
  return m_segmentParts.length();
}

CRGB *Segment::operator[] (uint16_t idx)
{
  uint16_t led = 0;
  for(uint8_t i = 0; i < m_segmentParts.length(); ++i) {
    const SegmentPart *part = m_segmentParts[i];
    if (led + part->size() > idx) {
      return (*part)[idx - led]; // found it!
    }
    led += part->size();
  }
  return nullptr;
}

uint16_t Segment::size()
{
  uint16_t sz = 0;
  for(uint16_t i = 0;  i < m_segmentParts.length(); ++i) {
    const SegmentPart *part = m_segmentParts[i];
    sz += part->size();
  }
  return sz;
}

// -----------------------------------------------------------

SegmentCompound::SegmentCompound() :
    SegmentCommon()
{
}

SegmentCompound::~SegmentCompound()
{
}

void SegmentCompound::addSegment(Segment *segment)
{
  m_segments.push(segment);
}

size_t SegmentCompound::segmentSize() const
{
  return m_segments.length();
}

void SegmentCompound::removeSegment(size_t idx)
{
  m_segments.remove(idx);
}

Segment* SegmentCompound::segmentAt(size_t idx)
{
  return m_segments[idx];
}

CRGB* SegmentCompound::operator [](uint16_t idx)
{
  uint16_t led = 0;
  for(uint8_t i = 0; i < m_segments.length(); ++i) {
    Segment *segment = m_segments[i];
    if (led + segment->size() > idx) {
      return (*segment)[idx - led]; // found it!
    }
    led += segment->size();
  }
  return nullptr;
}

uint16_t SegmentCompound::size()
{
  uint16_t sz = 0;
  for(uint16_t i = 0;  i < m_segments.length(); ++i) {
    Segment *segment = m_segments[i];
    sz += segment->size();
  }
  return sz;
}

