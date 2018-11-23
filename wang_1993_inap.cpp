/*
 * Copyright (C) 2011 Georgia Institute of Technology, University of Utah,
 * Weill Cornell Medical College
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This is a template implementation file for a user module derived from
 * DefaultGUIModel with a custom GUI.
 */

#include "wang_1993_inap.h"
#include <iostream>
#include <main_window.h>

extern "C" Plugin::Object*
createRTXIPlugin(void)
{
  return new Wang_1993Inap();
}

static DefaultGUIModel::variable_t vars[] = {

  {"v (V)", "Membrane potential", DefaultGUIModel::INPUT, },
  {"I (nA)", "Ionic current", DefaultGUIModel::OUTPUT, },
  {"g (uS)", "Conductance", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,},
  {"E (mV)", "Reversion potential", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,},
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

Wang_1993Inap::Wang_1993Inap(void)
  : DefaultGUIModel("Persistent sodium current from (Wang, 1993) neuron model", ::vars, ::num_vars)
{
  setWhatsThis("<p><b>Persistent sodium current from (Wang, 1993) neuron model.</p>");
  DefaultGUIModel::createGUI(vars,
                             num_vars); // this is required to create the GUI
  //customizeGUI();
  initParameters();
  update(INIT); // this is optional, you may place initialization code directly
                // into the constructor
  refresh();    // this is required to update the GUI with parameter and state
                // values
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

Wang_1993Inap::~Wang_1993Inap(void)
{
}

void
Wang_1993Inap::execute(void)
{
  double v = input(0) * 1000;
  double mnap = 1.0 / (1.0 + exp(- (v + 51.0) / 5.0));

  output(0) = g * mnap * (v - e);
  return;
}

void
Wang_1993Inap::initParameters(void)
{
  g = 0.1;
  e = 55;
}

void
Wang_1993Inap::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag) {
    case INIT:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      setParameter("g (uS)", g);
      setParameter("E (mV)", e);
      break;

    case MODIFY:
      g = getParameter("g (uS)").toDouble();
      e = getParameter("E (mV)").toDouble();
      break;

    case UNPAUSE:
      break;

    case PAUSE:
      break;

    case PERIOD:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      break;

    default:
      break;
  }
}

void
Wang_1993Inap::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  QGroupBox* button_group = new QGroupBox;

  QPushButton* abutton = new QPushButton("Button A");
  QPushButton* bbutton = new QPushButton("Button B");
  QHBoxLayout* button_layout = new QHBoxLayout;
  button_group->setLayout(button_layout);
  button_layout->addWidget(abutton);
  button_layout->addWidget(bbutton);
  QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));
  QObject::connect(bbutton, SIGNAL(clicked()), this, SLOT(bBttn_event()));

  customlayout->addWidget(button_group, 0, 0);
  setLayout(customlayout);
}

// functions designated as Qt slots are implemented as regular C++ functions
void
Wang_1993Inap::aBttn_event(void)
{
}

void
Wang_1993Inap::bBttn_event(void)
{
}
