/*
 * Actions.cpp
 *
 *  Created on: 16 apr 2019
 *      Author: jof
 */


#include "Actions.h"
#include "Leds.h"

ActionsContainer::ActionsContainer() :
  m_currentIdx(0)
{
}

ActionsContainer::~ActionsContainer()
{
}

size_t ActionsContainer::actionsSize() const
{
  return m_actions.length();
}

void ActionsContainer::actionsPush(ActionBase *action)
{
  m_actions.push(action);
}

void ActionsContainer::actionsRemove(ActionBase *action)
{
  for(uint16_t i = 0; i < m_actions.length(); ++i) {
    if (m_actions[i] == action) {
      m_actions.remove(i);
      if (m_currentIdx == i) {
        if (m_actions.length() -1 <= i)
          actionsRestart();
      } else if (m_currentIdx > i)
        --m_currentIdx;
    }
  }
}

void ActionsContainer::actionsRestart()
{
  m_currentIdx = 0;
  m_actions[m_currentIdx]->reset();
}

ActionBase* ActionsContainer::actionsCurrent()
{
  if (m_actions.length())
    return m_actions[m_currentIdx];
  return nullptr;
}

void ActionsContainer::nextAction()
{
  ++m_currentIdx;
  if (m_actions.length() == m_currentIdx)
    m_currentIdx = 0;

  m_actions[m_currentIdx]->reset();
}

// -------------------------------------------------------

ActionBase::ActionBase(uint32_t duration) :
  m_singleShot(false), m_startTime(0),
  m_duration(duration)
{
}

// how many ms between each re-render
const uint8_t ActionBase::m_updateTime = 70;

ActionBase::~ActionBase()
{
}

bool ActionBase::isRunning() const
{
  return m_startTime > 0 && !isFinished();
}

bool ActionBase::isFinished() const
{
  // duration of 0 is forever
  return m_duration > 0 && m_startTime + m_duration >= millis();
}

void ActionBase::reset()
{
  m_startTime = 0;
}

void ActionBase::loop(SegmentCommon *owner)
{
  if (m_startTime == 0)
    m_startTime = millis();

  if (isFinished()) {
    if (m_singleShot)
      owner->actionsRemove(this); // caution, deletes this,
                                  // no code execution after this line
    else
      owner->nextAction();
  }
}

// --------------------------------------------------

ActionColor::ActionColor(CRGB color, uint32_t duration) :
    ActionBase(duration),
    m_color(color)
{
}

ActionColor::~ActionColor()
{
}

void ActionColor::loop(SegmentCommon *owner)
{
  if (m_startTime == 0) {
    for(uint16_t i = 0, end = owner->size(); i < end; ++i) {
      CRGB *rgb = (*owner)[i];
      *rgb = m_color;
    }
    owner->render();
  }

  ActionBase::loop(owner);
}

// -----------------------------------------------

ActionDark::ActionDark(uint32_t duration) :
    ActionColor(CRGB::Black, duration)
{
}

ActionDark::~ActionDark()
{
}

// -----------------------------------------------
ActionIncColor::ActionIncColor(CRGB leftColor, CRGB rightColor, uint32_t duration) :
    ActionBase(duration),
    m_leftColor(leftColor), m_rightColor(rightColor)
{
}

ActionIncColor::~ActionIncColor()
{
}

void ActionIncColor::loop(SegmentCommon *owner)
{
  if (m_startTime == 0) {
    int16_t diff[3] = {
        m_leftColor.red - m_rightColor.red,
        m_leftColor.green - m_rightColor.green,
        m_leftColor.blue  - m_rightColor.blue
    };

    for (uint8_t c = 0; c < 3; ++c) {
      // iterate for each color
      for(uint16_t i = 0, sz = owner->size(); i < sz; ++i) {
        float color = diff[c] / sz;
        color = m_leftColor.raw[c] + (color * i);
        CRGB *rgb = (*owner)[i];
        rgb->raw[c] = round(color);
      }
    }
    owner->render();
  }

  ActionBase::loop(owner);
}


// -----------------------------------------------

ActionDimAll::ActionDimAll(CRGB fromColor, CRGB toColor, uint32_t duration) :
    ActionBase(duration),
    m_fromColor(fromColor), m_toColor(toColor),
    m_nextIterTime(m_updateTime)
{
  // calculate how far away each color is and how much they should change each 70ms
  int16_t redDiff   = toColor.red - fromColor.red,
          greenDiff = toColor.green - fromColor.green,
          blueDiff  = toColor.blue  - fromColor.blue;
  int16_t maxDiff = max(max(redDiff, greenDiff), blueDiff),
          minDiff = min(min(redDiff, greenDiff), blueDiff);
  if ((-maxDiff) > minDiff)
    // darker color wins
    m_iterations = -minDiff / (duration || 1); // negate minDiff as we don't want
                                               // negative m_iterations
  else
    m_iterations = maxDiff / (duration || 1);

  m_incR = (redDiff / m_iterations) || 1;
  m_incG = (greenDiff / m_iterations) || 1;
  m_incB = (blueDiff / m_iterations) || 1;
}

ActionDimAll::~ActionDimAll()
{
}

void ActionDimAll::loop(SegmentCommon *owner)
{
  uint8_t r, g, b;
  if (m_startTime == 0) {
    r = m_fromColor.red;
    g = m_fromColor.green;
    b = m_fromColor.blue;
  } else {
    r = m_toColor.red - (m_iterations * m_incR);
    g = m_toColor.green - (m_iterations * m_incG);
    b = m_toColor.blue - (m_iterations * m_incB);
  }

  if (m_startTime == 0 || m_startTime + m_nextIterTime <= millis()) {
    m_nextIterTime = millis() + m_updateTime;
    for(uint16_t i = 0, end = owner->size(); i < end; ++i) {
      CRGB *rgb = (*owner)[i];
      rgb->red = r;
      rgb->green = g;
      rgb->blue = b;
    }
    owner->render();
  }

  // call subclass
  ActionBase::loop(owner);
}

void ActionDimAll::reset()
{
  m_nextIterTime = m_updateTime;
  ActionBase::reset();
}




