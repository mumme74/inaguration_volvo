/*
 * Actions.cpp
 *
 *  Created on: 16 apr 2019
 *      Author: jof
 */

#include "Actions.h"

ActionBase::ActionBase(Segment *segment) :
  m_singleShot(false), m_time(0),
  m_segment(segment), m_segmentCompound(nullptr)
{
}

ActionBase::ActionBase(SegmentCompound *compound) :
  m_singleShot(false), m_time(0),
  m_segment(nullptr), m_segmentCompound(compound)
{
}

ActionBase::~ActionBase()
{
}

bool ActionBase::isRunning() const
{
  return m_time > 0 && !isFinished();
}
