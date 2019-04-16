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

Segment::Segment() :
    m_halted(true)
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

void Segment::setHalted(bool halt)
{
  m_halted = halt;
}

// -----------------------------------------------------------

SegmentCompound::SegmentCompound()
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

Segment* SegmentCompound::operator [](size_t idx)
{
  return m_segments[idx];
}

