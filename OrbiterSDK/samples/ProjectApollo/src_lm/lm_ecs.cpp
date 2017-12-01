/***************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2004-2007

  Environmental Control System

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************************************************************/

#include "Orbitersdk.h"
#include "soundlib.h"
#include "toggleswitch.h"
#include "apolloguidance.h"
#include "LEMcomputer.h"
#include "LEM.h"
#include "lm_ecs.h"

LEM_ECS::LEM_ECS(PanelSDK &p) : sdk(p)
{
	lem = NULL;
	// Initialize
	Asc_Oxygen1 = 0;
	Asc_Oxygen2 = 0;
	Des_Oxygen = 0;
	//Des_Oxygen2 = 0; Using LM-8 Systems Handbook, only 1 DES O2 tank
	Asc_Water1 = 0;
	Asc_Water2 = 0;
	Des_Water = 0;
	//Des_Water2 = 0; Using LM-8 Systems Handbook, only 1 DES H2O tank
	Primary_CL_Glycol_Press = 0; // Zero this, system will fill from accu
	Secondary_CL_Glycol_Press = 0;  // Zero this, system will fill from accu
	Primary_CL_Glycol_Temp = 0;  // 40 in the accu, 0 other side of the pump
	Secondary_CL_Glycol_Temp = 0; // 40 in the accu, 0 other side of the pump
	Primary_Glycol_Accu = 0; // Cubic inches of coolant
	Secondary_Glycol_Accu = 0; // Cubic inches of coolant
	Primary_Glycol = 0;
	Secondary_Glycol = 0;
	// Open valves as would be for IVT
	Des_O2 = 0;
	Des_H2O_To_PLSS = 0;
	Cabin_Repress = 0; // Auto
					   // For simplicity's sake, we'll use a docked LM as it would be at IVT, at first docking the LM is empty!
	Cabin_Press = 0; Cabin_Temp = 0; Cabin_CO2 = 0;
	Suit_Press = 0; Suit_Temp = 0; Suit_CO2 = 0;

}

void LEM_ECS::Init(LEM *s) {
	lem = s;
}

void LEM_ECS::TimeStep(double simdt) {
	if (lem == NULL) { return; }
	// **** Atmosphere Revitalization Section ****
	// First, get air from the suits and/or the cabin into the system.
	// Second, remove oxygen for and add CO2 from the crew.
	// Third, remove CO2 from the air and simulate the reaction in the LiOH can
	// Fourth, use the fans to move the resulting air through the suits and/or the cabin.
	// Fifth, use the heat exchanger to move heat from the air to the HTS if enabled (emergency ops)
	// Sixth, use the water separators to remove water from the air and add it to the WMS and surge tank.
	// Seventh, use the OSCPCS to add pressure if required
	// Eighth, use the regenerative heat exchanger to add heat to the air if required
	// Ninth and optionally, simulate the system behavior if a PGA failure is detected.
	// Tenth, simulate the LCG water movement operation.

	// **** Oxygen Supply and Cabin Pressure Control Section ****
	// Simple, move air from tanks to the cabin as required, and move air from the cabin to space as required.

	// **** Water Management Section ****
	// Also relatively simple, move water from tanks to the HTS / crew / etc as required.

	// **** Heat Transport Section ****
	// First, operate pumps to move glycol/water through the loops.
	// Second, move heat from the equipment to the glycol.
	// Third, move heat from the glycol to the sublimators.
	// Fourth, vent steam from the sublimators overboard.
}

void LEM_ECS::SaveState(FILEHANDLE scn, char *start_str, char *end_str) {

}

void LEM_ECS::LoadState(FILEHANDLE scn, char *end_str) {

}

double LEM_ECS::DescentOxyTankPressure() {
	if (!Des_OxygenPress) {
		Des_OxygenPress = (double*)sdk.GetPointerByString("HYDRAULIC:DESO2TANK:PRESS");
	}
	return *Des_OxygenPress;
}

double LEM_ECS::DescentOxyTankPressurePSI() {
	return DescentOxyTankPressure() * PSI;
}

double LEM_ECS::AscentOxyTank1Pressure() {
	if (!Asc_Oxygen1Press) {
		Asc_Oxygen1Press = (double*)sdk.GetPointerByString("HYDRAULIC:ASCO2TANK1:PRESS");
	}
	return *Asc_Oxygen1Press;
}

double LEM_ECS::AscentOxyTank1PressurePSI() {
	return AscentOxyTank1Pressure() * PSI;
}

double LEM_ECS::AscentOxyTank2Pressure() {
	if (!Asc_Oxygen2Press) {
		Asc_Oxygen2Press = (double*)sdk.GetPointerByString("HYDRAULIC:ASCO2TANK2:PRESS");
	}
	return *Asc_Oxygen2Press;
}

double LEM_ECS::AscentOxyTank2PressurePSI() {
	return AscentOxyTank2Pressure() * PSI;
}

double LEM_ECS::DescentOxyTankQuantity() {
	if (!Des_Oxygen) {
		Des_Oxygen = (double*)sdk.GetPointerByString("HYDRAULIC:DESO2TANK:MASS");
	}
	return *Des_Oxygen;
}

double LEM_ECS::AscentOxyTank1Quantity() {
	if (!Asc_Oxygen1) {
		Asc_Oxygen1 = (double*)sdk.GetPointerByString("HYDRAULIC:ASCO2TANK1:MASS");
	}
	return *Asc_Oxygen1;
}

double LEM_ECS::AscentOxyTank2Quantity() {
	if (!Asc_Oxygen2) {
		Asc_Oxygen2 = (double*)sdk.GetPointerByString("HYDRAULIC:ASCO2TANK2:MASS");
	}
	return *Asc_Oxygen2;
}

double LEM_ECS::GetCabinPressure() {
	if (!Cabin_Press) {
		Cabin_Press = (double*)sdk.GetPointerByString("HYDRAULIC:CABIN:PRESS");
	}
	return *Cabin_Press;
}

double LEM_ECS::GetCabinPressurePSI() {
	return GetCabinPressure() * PSI;
}

double LEM_ECS::GetCabinCO2MMHg() {
	if (!Cabin_CO2) {
		Cabin_CO2 = (double*)sdk.GetPointerByString("HYDRAULIC:CABIN:CO2_PPRESS");
	}
	return *Cabin_CO2 * MMHG;
}

double LEM_ECS::GetSuitPressure() {
	if (!Suit_Press) {
		Suit_Press = (double*)sdk.GetPointerByString("HYDRAULIC:SUITCIRCUIT:PRESS");
	}
	return *Suit_Press;
}

double LEM_ECS::GetSuitPressurePSI() {
	return GetSuitPressure() * PSI;
}

double LEM_ECS::GetSuitCO2MMHg() {
	if (!Suit_CO2) {
		Suit_CO2 = (double*)sdk.GetPointerByString("HYDRAULIC:SUIT:CO2_PPRESS");
	}
	return *Suit_CO2 * MMHG;
}

double LEM_ECS::DescentWaterTankQuantityLBS() {
	if (!Des_Water) {
		Des_Water = (double*)sdk.GetPointerByString("HYDRAULIC:DESH2OTANK:MASS");
	}
	return *Des_Water * 0.0022046226218;  //grams to pounds
}

double LEM_ECS::AscentWaterTank1QuantityLBS() {
	if (!Asc_Water1) {
		Asc_Water1 = (double*)sdk.GetPointerByString("HYDRAULIC:ASCH2OTANK1:MASS");
	}
	return *Asc_Water1 * 0.0022046226218;  //grams to pounds
}

double LEM_ECS::AscentWaterTank2QuantityLBS() {
	if (!Asc_Water2) {
		Asc_Water2 = (double*)sdk.GetPointerByString("HYDRAULIC:ASCH2OTANK2:MASS");
	}
	return *Asc_Water2 * 0.0022046226218;  //grams to pounds
}

double LEM_ECS::GetCabinTemperature() {
	if (!Cabin_Temp) {
		Cabin_Temp = (double*)sdk.GetPointerByString("HYDRAULIC:CABIN:TEMP");
	}
	return *Cabin_Temp * 1.8 - 459.67;   //K to F
}

double LEM_ECS::GetSuitTemperature() {
	if (!Suit_Temp) {
		Suit_Temp = (double*)sdk.GetPointerByString("HYDRAULIC:SUITCIRCUIT:TEMP");
	}
	return *Suit_Temp * 1.8 - 459.67;   //K to F
}

double LEM_ECS::GetPrimaryGlycolPressure() {
	if (!Primary_CL_Glycol_Press) {
		Primary_CL_Glycol_Press = (double*)sdk.GetPointerByString("HYDRAULIC:PRIMGLYCOLLOOP:PRESS");
	}
	return *Primary_CL_Glycol_Press;
}

double LEM_ECS::GetPrimaryGlycolTemperature() {
	if (!Primary_CL_Glycol_Temp) {
		Primary_CL_Glycol_Temp = (double*)sdk.GetPointerByString("HYDRAULIC:PRIMGLYCOLLOOP:TEMP");
	}
	return *Primary_CL_Glycol_Temp;
}

double LEM_ECS::GetSecondaryGlycolPressure() {
	if (!Secondary_CL_Glycol_Press) {
		Secondary_CL_Glycol_Press = (double*)sdk.GetPointerByString("HYDRAULIC:SECGLYCOLLOOP:PRESS");
	}
	return *Secondary_CL_Glycol_Press;
}

double LEM_ECS::GetSecondaryGlycolTemperature() {
	if (!Secondary_CL_Glycol_Temp) {
		Secondary_CL_Glycol_Temp = (double*)sdk.GetPointerByString("HYDRAULIC:SECGLYCOLLOOP:TEMP");
	}
	return *Secondary_CL_Glycol_Temp;
}
