#ifndef __ForwardTool_h__
#define __ForwardTool_h__
// ForwardTool.h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <OpenSim/Common/Object.h>
#include <OpenSim/Common/PropertyBool.h>
#include <OpenSim/Common/PropertyStr.h>
#include <OpenSim/Common/PropertyInt.h>
#include <OpenSim/Common/PropertyDblArray.h>
#include <OpenSim/Common/Storage.h>
#include <OpenSim/Simulation/Model/AbstractTool.h>
#include <OpenSim/Simulation/Control/ControlSet.h>
#include "osimToolsDLL.h"

#ifdef SWIG
	#ifdef OSIMTOOLS_API
		#undef OSIMTOOLS_API
		#define OSIMTOOLS_API
	#endif
#endif

namespace OpenSim { 

class AbstractBody;
class ForceApplier;
class TorqueApplier;
class LinearSpring;
class TorsionalSpring;
class ModelIntegrand;

//=============================================================================
//=============================================================================
/**
 * An abstract class for specifying the interface for an investigation.
 *
 * @author Frank C. Anderson
 * @version 1.0
 */
class OSIMTOOLS_API ForwardTool: public AbstractTool
{
//=============================================================================
// MEMBER VARIABLES
//=============================================================================
protected:
	// BASIC INPUT
	/** Name of the controls file. */
	PropertyStr _controlsFileNameProp;
	std::string &_controlsFileName;
	/** Name of the states file.  The states file must at a minimum contain the
	initial states for a simulation.  If a complete set of states is available,
	the time stamps can be used to specify the integration steps and corrective
	springs, which allow perturbations, can be added to the simulation. */
	PropertyStr _statesFileNameProp;
	std::string &_statesFileName;
	/** If true, the time steps from the states file are used during
	current integration. */
	OpenSim::PropertyBool _useSpecifiedDtProp;
	bool &_useSpecifiedDt;

	// EXTERNAL LOADS
	/** Name of the file containing the external loads applied to the model. */
	OpenSim::PropertyStr _externalLoadsFileNameProp;
	std::string &_externalLoadsFileName;
	/** Name of the file containing the model kinematics corresponding to the
	external loads. */
	OpenSim::PropertyStr _externalLoadsModelKinematicsFileNameProp;
	std::string &_externalLoadsModelKinematicsFileName;
	/** Name of the body to which the first set of external loads should be
	applied (e.g., the body name for the right foot). */
	OpenSim::PropertyStr _externalLoadsBody1Prop;
	std::string &_externalLoadsBody1;
	/** Name of the body to which the second set of external loads should be
	applied (e.g., the body name for the left foot). */
	OpenSim::PropertyStr _externalLoadsBody2Prop;
	std::string &_externalLoadsBody2;
	/** Low-pass cut-off frequency for filtering the model kinematics corresponding
	to the external loads. A negative value results in no filtering.
	The default value is -1.0, so no filtering. */
	OpenSim::PropertyDbl _lowpassCutoffFrequencyForLoadKinematicsProp;
	double &_lowpassCutoffFrequencyForLoadKinematics;
	/** Flag indicating whether or not to output corrective spring loads and other
	quantities. */
	OpenSim::PropertyBool _outputDetailedResultsProp;
	bool &_outputDetailedResults;

	// FOOT CONTACT EVENT TIMES
	/** Flag indicating wether or not to turn on a linear corrective spring for the right foot. */
	OpenSim::PropertyBool _rLinSpringOnProp;
	bool &_rLinSpringOn;
	/** Flag indicating wether or not to turn on a torsional corrective spring for the right foot. */
	OpenSim::PropertyBool _rTorSpringOnProp;
	bool &_rTorSpringOn;
	/** Flag indicating wether or not to turn on a linear corrective spring for the left foot. */
	OpenSim::PropertyBool _lLinSpringOnProp;
	bool &_lLinSpringOn;
	/** Flag indicating wether or not to turn on a torsional corrective spring for the left foot. */
	OpenSim::PropertyBool _lTorSpringOnProp;
	bool &_lTorSpringOn;
	/** Time of right heel strike. */
	PropertyDbl _rHeelStrikeProp;
	double &_rHeelStrike;
	/** Time of right foot flat. */
	PropertyDbl _rFootFlatProp;
	double &_rFootFlat;
	/** Time of right heel off. */
	PropertyDbl _rHeelOffProp;
	double &_rHeelOff;
	/** Time of right toe off. */
	PropertyDbl _rToeOffProp;
	double &_rToeOff;
	/** Time of left heel strike. */
	PropertyDbl _lHeelStrikeProp;
	double &_lHeelStrike;
	/** Time of left foot flat. */
	PropertyDbl _lFootFlatProp;
	double &_lFootFlat;
	/** Time of left heel off. */
	PropertyDbl _lHeelOffProp;
	double &_lHeelOff;
	/** Time of left toe off. */
	PropertyDbl _lToeOffProp;
	double &_lToeOff;

	// CORRECTIVE SPRING PARAMETERS
	/** Rise time for scaling functions. */
	PropertyDbl _tauProp;
	double &_tau;
	PropertyDbl _tauRightStartProp;
	double &_tauRightStart;
	PropertyDbl _tauRightEndProp;
	double &_tauRightEnd;
	PropertyDbl _tauLeftStartProp;
	double &_tauLeftStart;
	PropertyDbl _tauLeftEndProp;
	double &_tauLeftEnd;
	/** Spring transition weight. */
	PropertyDbl _springTransitionStartForceProp;
	double &_springTransitionStartForce;
	PropertyDbl _springTransitionEndForceProp;
	double &_springTransitionEndForce;
	/** Stiffness for linear corrective springs. */
	PropertyDblArray _kLinProp;
	Array<double> &_kLin;
	/** Damping for linear corrective springs. */
	PropertyDblArray _bLinProp;
	Array<double> &_bLin;
	/** Stiffness for torsional corrective springs. */
	PropertyDblArray _kTorProp;
	Array<double> &_kTor;
	/** Damping for torsional corrective springs. */
	PropertyDblArray _bTorProp;
	Array<double> &_bTor;

	// INTERNAL WORK ARRAYS
	/** Model integrand.  Make it a pointer so we can print results from a separate function. */
	ModelIntegrand *_integrand;
	/** Storage for the input states. */
	Storage *_yStore;
	/** Flag indicating whether or not to write to the results (GUI will set this to false). */
	bool _printResultFiles;
	/** Pointer to the linear and torsional corrective springs. */
	LinearSpring *_rLin, *_lLin;
	TorsionalSpring *_rTor, *_lTor;

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
public:
	virtual ~ForwardTool();
	ForwardTool();
	ForwardTool(const std::string &aFileName, bool aLoadModel=true) SWIG_DECLARE_EXCEPTION;
	ForwardTool(const ForwardTool &aObject);
	virtual Object* copy() const;
private:
	void setNull();
	void setupProperties();
protected:
	//void constructCorrectiveSprings(ForceApplier *aRightGRFApp,ForceApplier *aLeftGRFApp);
	LinearSpring*
		addLinearCorrectiveSpring(const Storage &aQStore,const Storage &aUStore,const ForceApplier &aAppliedForce);
	TorsionalSpring*
		addTorsionalCorrectiveSpring(const Storage &aQStore,const Storage &aUStore,AbstractBody *aBody,
		double aTauOn,double aTimeOn,double aTauOff,double aTimeOff);

	//--------------------------------------------------------------------------
	// OPERATORS
	//--------------------------------------------------------------------------
public:
#ifndef SWIG
	ForwardTool&
		operator=(const ForwardTool &aForwardTool);
#endif

	//--------------------------------------------------------------------------
	// GET AND SET
	//--------------------------------------------------------------------------

	const std::string &getControlsFileName() const { return _controlsFileName; }
	void setControlsFileName(const std::string &aFileName) { _controlsFileName = aFileName; }

	const std::string &getStatesFileName() const { return _statesFileName; }
	void setStatesFileName(const std::string &aFileName) { _statesFileName = aFileName; }

	bool getUseSpecifiedDt() const { return _useSpecifiedDt; }
	void setUseSpecifiedDt(bool aUseSpecifiedDt) { _useSpecifiedDt = aUseSpecifiedDt; }

	// External loads get/set
	const std::string &getExternalLoadsFileName() const { return _externalLoadsFileName; }
	void setExternalLoadsFileName(const std::string &aFileName) { _externalLoadsFileName = aFileName; }
	const std::string &getExternalLoadsModelKinematicsFileName() const { return _externalLoadsModelKinematicsFileName; }
	void setExternalLoadsModelKinematicsFileName(const std::string &aFileName) { _externalLoadsModelKinematicsFileName = aFileName; }
	const std::string &getExternalLoadsBody1() const { return _externalLoadsBody1; }
	void setExternalLoadsBody1(const std::string &aName) { _externalLoadsBody1 = aName; }
	const std::string &getExternalLoadsBody2() const { return _externalLoadsBody2; }
	void setExternalLoadsBody2(const std::string &aName) { _externalLoadsBody2 = aName; }
	double getLowpassCutoffFrequencyForLoadKinematics() const { return _lowpassCutoffFrequencyForLoadKinematics; }
	void setLowpassCutoffFrequencyForLoadKinematics(double aLowpassCutoffFrequency) { _lowpassCutoffFrequencyForLoadKinematics = aLowpassCutoffFrequency; }
	void setPrintResultFiles(bool aToWrite) { _printResultFiles = aToWrite; }

	//--------------------------------------------------------------------------
	// INTERFACE
	//--------------------------------------------------------------------------
	virtual bool run();
	void printResults();

	//--------------------------------------------------------------------------
	// UTILITY
	//--------------------------------------------------------------------------
	static void initializeExternalLoads(Model *aModel, 
		const std::string &aExternalLoadsFileName,
		const std::string &aExternalLoadsModelKinematicsFileName,
		const std::string &aExternalLoadsBody1,
		const std::string &aExternalLoadsBody2,
		double aLowpassCutoffFrequencyForLoadKinematics,
		ForceApplier **rRightForceApp=0,
		ForceApplier **rLeftForceApp=0,
		TorqueApplier **rRightTorqueApp=0,
		TorqueApplier **rLeftTorqueApp=0);

//=============================================================================
};	// END of class ForwardTool

}; //namespace
//=============================================================================
//=============================================================================

#endif // __ForwardTool_h__


