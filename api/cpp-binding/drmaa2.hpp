/*
 * Copyright (C) 1994-2017 Altair Engineering, Inc.
 * For more information, contact Altair at www.altair.com.
 *
 * This file is part of the PBS Professional ("PBS Pro") software.
 *
 * Open Source License Information:
 *
 * PBS Pro is free software. You can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Commercial License Information:
 *
 * The PBS Pro software is licensed under the terms of the GNU Affero General
 * Public License agreement ("AGPL"), except where a separate commercial license
 * agreement for PBS Pro version 14 or later has been executed in writing with Altair.
 *
 * Altair’s dual-license business model allows companies, individuals, and
 * organizations to create proprietary derivative works of PBS Pro and distribute
 * them - whether embedded or bundled with other software - under a commercial
 * license agreement.
 *
 * Use of Altair’s trademarks, including but not limited to "PBS™",
 * "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's
 * trademark licensing policies.
 *
 */

#ifndef DRMAA2_HPP_
#define DRMAA2_HPP_
#include <config.h>
#include <ctime>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <map>

#ifdef DRMAA2_DEBUG
void drmaa2_cpptest();
#endif

using namespace std;

/**
 * @namespace drmaa2 interfaces and structures are encapsulated in
 * 				single namespace
 */
namespace drmaa2 {

typedef long long TimeAmount;
typedef time_t AbsoluteTime;

/**
 * @enum DrmaaCapability
 *
 * @brief Expresses DRMAA features and data attributes that
 * 			may or may not be supported by a PBSPro.
 */
enum DrmaaCapability {
	ADVANCE_RESERVATION,
	RESERVE_SLOTS,
	CALLBACK,
	BULK_JOBS_MAXPARALLEL,
	JT_EMAIL,
	JT_STAGING,
	JT_DEADLINE,
	JT_MAXSLOTS,
	JT_ACCOUNTINGID,
	RT_STARTNOW,
	RT_DURATION,
	RT_MACHINEOS,
	T_MACHINEARCH
};

/**
 * @enum JobState
 *
 * @brief enumeration reports the jobs current state according
 * 			to the DRMAA job state model
 *
 */
enum JobState {
	UNDETERMINED,
	QUEUED,
	QUEUED_HELD,
	RUNNING,
	SUSPENDED,
	REQUEUED,
	REQUEUED_HELD,
	DONE,
	FAILED
};

/**
 * @enum OperatingSystem
 *
 * @brief enumeration defines a set of standardized identifiers
 * 			for operating system types.
 *
 */
enum OperatingSystem {
	AIX = 0,
	BSD = 1,
	LINUX = 2,
	HPUX = 3,
	IRIX = 4,
	MACOS = 5,
	SUNOS = 6,
	TRUE64 = 7,
	UNIXWARE = 8,
	WI = 9,
	WINNT = 10,
	OTHER_OS = 11
};

/**
 * @enum OperatingSystem
 *
 * @brief enumeration is used as data type in job submission,
 * 			advance reservation and system monitoring.
 *
 */

enum CpuArchitecture {
	ALPHA = 0,
	ARM = 1,
	CELL = 2,
	PARISC = 3,
	X86 = 4,
	X64 = 5,
	IA64 = 6,
	MIPS = 7,
	PPC = 8,
	PPC64 = 9,
	SPARC = 10,
	SPARC64 = 11,
	OTHER_CPU = 12
};

/**
 * @enum OperatingSystem
 * @brief enumeration defines standard event types for notification.
 *
 */

enum DrmaaEvent {
	NEW_STATE, MIGRATED, ATTRIBUTE_CHANGE
};

/**
 * DRMAA2 root specification predefined string constants
 */
extern const char *const DRMAA2_CORE_FILE_SIZE;
extern const char *const DRMAA2_CPU_TIME;
extern const char *const DRMAA2_DATA_SIZE;
extern const char *const DRMAA2_FILE_SIZE;
extern const char *const DRMAA2_OPEN_FILES;
extern const char *const DRMAA2_STACK_SIZE;
extern const char *const DRMAA2_VIRTUAL_MEMORY;
extern const char *const DRMAA2_WALLCLOCK_TIME;

typedef list<string> Dictionary;
typedef list<string> StringList;
typedef list<string> OrderedStringList;

/**
 * @struct DrmaaNotification
 * @brief defines attributes for a DrmaaCallback
 *
 * */
struct DrmaaNotification {
	DrmaaEvent event; /*!< Standard event types for notification */
	string jobId; /*!< Unique Job identification in DRMS */
	string sessionName; /*!< SessionName from which job is submitted */
	JobState jobState; /*!< Job State */
};

/**
 * @class DrmaaCallback
 * @brief Abstract class allows the DRMAA library or the DRMS
 * 			system to inform the application about relevant events in an
 * 			asynchronous fashion. One expected use case is continuous monitoring
 * 			of job state transitions.
 * */
class DrmaaCallback {
public:
	virtual ~DrmaaCallback();
	virtual void notify(DrmaaNotification notification) = 0;
};

/**
 * @struct Version
 * @brief defines version information
 *
 * */
struct Version {
	Version(string major_, string minor_) :
			major(major_), minor(minor_) {
		//DO nothing
	}
	string major; /*!< Major number of version*/
	string minor; /*!< Minor number of version*/
};

/**
 * @struct ReservationTemplate
 * @brief defines the characteristics of a reported advance
 * 			reservation, the DRMAA application creates an ReservationTemplate
 * 			instance and submits it through the ReservationSession methods.
 *
 */
struct ReservationTemplate {
	string reservationName; /*!< Reservation name*/
	time_t startTime; /*!< Start time of reservation*/
	time_t endTime; /*!< End time of reservation*/
	TimeAmount duration; /*!< Duration of reservation */
	long minSlots; /*!< Minimum slots*/
	long maxSlots; /*!< Maximum slots*/
	vector<string> usersACL; /*!< user access control list for reservation*/
	vector<string> candidateMachines; /*!< List of machines for reservations*/
	long minPhysMemory; /*!< Minimum physical required */
	OperatingSystem machineOS; /*!< Machine OS*/
	CpuArchitecture machineArch; /*!< Machine Cpu Architecture*/
};

/**
 * @struct QueueInfo
 * @brief defines queues associated with DRMS( PBSPro)
 *
 */
struct QueueInfo {
	string name; /*!< Queue name*/
};

typedef list<QueueInfo> QueueInfoList;

/**
 * @struct JobInfo
 * @brief provides detailed information about the characteristics of a Job
 *
 */
struct JobInfo {
	string jobId; /*!< Unique Job identification in DRMS */
	long exitStatus; /*!< Exit status of Job*/
	string terminatingSignal; /*!< Signal */
	string annotation; /*!< //TODO */
	JobState jobState; /*!< Job State*/
	string jobSubState; /*!< Job sub state */
	vector<string> allocatedMachines; /*!< allotted machine to run job*/
	string submissionMachine; /*!< Machine from which job is submitted*/
	string jobOwner; /*!< Job Owner*/
	long slots; /*!< Slots*/
	string queueName; /*!< Queue name */
	time_t wallclockTime; /*!< Wall time*/
	long cpuTime; /*!< CPU time*/
	time_t submissionTime; /*!< Submission time*/
	time_t dispatchTime; /*!< Dispatch time*/
	time_t finishTime; /*!< Finish time*/
};
typedef list<JobInfo> JobInfoList;

/**
 * @struct SlotInfo
 * @brief provides detailed information slots of DMRS(PBSPro).
 *
 */
struct SlotInfo {
	string machineName; /*!< Machine name*/
	string slots; /*!< Slots*/
};
typedef list<SlotInfo> SlotInfoList;

/**
 * @struct ReservationInfo
 * @brief provides information about an existing advance reservation,
 * 			as reported by the DRMS.
 */
struct ReservationInfo {
	string reservationId; /*!< Unique Reservation identification in DRMS */
	string reservationName; /*!< Reservation name */
	time_t reservedStartTime; /*!< Reservation start time*/
	time_t reservedEndTime; /*!< Reservation end time) */
	set<string> usersACL; /*!< User access control list of reservation*/
	long reservedSlots; /*!< Slots reserved for reservation*/
	SlotInfoList reservedMachines; /*!< Reservation machines*/
};
typedef list<ReservationInfo> ReservationInfoList;

/**
 * @struct JobTemplate
 * @brief defines characteristics of a job submission.
 *
 */
struct JobTemplate {
	string remoteCommand; /*!< Remote command*/
	vector<string> args; /*!< Job arguments */
	bool submitAsHold; /*!< Flag indicating job to be submitted but not to run*/
	bool rerunnable; /*!< Flag indicating job is rerunnable or not*/
	map<string, string> jobEnvironment; /*!< List of job envs*/
	string workingDirectory; /*!< Jobs working directory*/
	string jobCategory; /*!< Job Category*/
	list<string> email; /*!< Email id */
	bool emailOnStarted; /*!< Flag indicating whether to send mail on start or not */
	bool emailOnTerminated; /*!< Flag indicating whether to send mail on end or not */
	string jobName; /*!< User defined job */
	string inputPath; /*!< Input path*/
	string outputPath; /*!< Output path*/
	string errorPath; /*!< Error path*/
	bool joinFiles; /*!< Join files*/
	string reservationId; /*!< Reservation id*/
	string queueName; /*!< Queue name*/
	long minSlots; /*!< Minimum slots*/
	long maxSlots; /*!< Maximum slots*/
	long priority; /*!< Job priority*/
	vector<string> candidateMachines; /*!< List of candidate names to run job*/
	long minPhysMemory; /*!< Minimum memory required for job to run*/
	OperatingSystem machineOS; /*!< Operating system in which job should run*/
	CpuArchitecture machineArch; /*!< CPU architecture in which job should run*/
	time_t startTime; /*!< When to start job*/
	time_t deadlineTime; /*!< Dead line at which job should finish*/
	map<string, string> stageInFiles; /*!< Stage in files*/
	map<string, string> stageOutFiles; /*!< Stage out files*/
	map<string, string> resourceLimits; /*!< Resource limits*/
	string accountingId; /*!< Account id*/
};

/**
 * @struct MachineInfo
 * @brief describes the properties of a particular
 * 			execution host in the DRMS
 */
struct MachineInfo {
	MachineInfo(string major_ = string(), string minor_ = string()) : machineOSVersion(major_, minor_) {
	}
	string name; /*!< Machine Name*/
	bool available; /*!< Machine Availability*/
	long sockets; /*!< Number of sockets in Machine*/
	long coresPerSocket; /*!< Cores per socket*/
	long threadsPerCore; /*!< Threads per core*/
	double load; /*!< Load on machine*/
	long physMemory; /*!< Physical memory*/
	long virtMemory; /*!< Virtual Name*/
	OperatingSystem machineOS; /*!< OS of machine */
	Version machineOSVersion; /*!< OS version of machine*/
	CpuArchitecture machineArch; /*!< CPU architecture of version*/
};
typedef list<MachineInfo> MachineInfoList;

/**
 * List of supported exceptions
 */
class Drmaa2Exception;
class DeniedByDrmsException;
class DrmCommunicationException;
class TryLaterException;
class TimeoutException;
class InternalException;
class InvalidArgumentException;
class InvalidSessionException;
class InvalidStateException;
class OutOfResourceException;
class UnsupportedAttributeException;
class UnsupportedOperationException;
class ImplementationSpecificException;

/**
 * @class DrmaaReflective
 * @brief TODO add the comments here
 *
 */
class DrmaaReflective {
	/**
	 * TODO : How to support reflective in C++?
	 * Java provides reflection
	 */
	StringList jobTemplateImplSpec;
	StringList jobInfoImplSpec;
	StringList reservationTemplateImplSpec;
	StringList reservationInfoImplSpec;
	StringList queueInfoImplSpec;
	StringList machineInfoImplSpec;
	StringList notificationImplSpec;
	string getInstanceValue(void *instance, string name);
	void setInstanceValue(void *instance, string name, string value);
	string describeAttribute(void *instance, string name);
};

/**
 * @class Job
 * @brief Abstract class allows one to instruct the DRMS
 * 			of a job status change, and to query the properties of the
 * 			job in the DRMS
 *
 */
class Job {
public:

	/**
	 * Destructor
	 */
	virtual ~Job(void) {
	};

	/**
	 * @brief Returns Job ID
	 *
	 * @param - None
	 *
	 * @return valid job ID
	 */
	virtual const string& getJobId(void) const = 0;

	/**
	 * @brief Returns detailed Job information
	 *
	 * @param - None
	 *
	 * @return JobInfo
	 */
	virtual const JobInfo& getJobInfo(void) const = 0;

	/**
	 * @brief Returns Job State
	 *
	 * @param[out] subState - Sub state of job is set
	 *
	 * @return JobState
	 */
	virtual const JobState& getState(string& subState) = 0;

	/**
	 * @brief Returns JobTemplate from which Job is submitted
	 *
	 * @param - None
	 *
	 * @return JobTemlpate
	 */
	virtual const JobTemplate& getJobTemplate(void) const = 0;

	/**
	 * @brief Suspends a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void suspend(void) const throw() = 0;

	/**
	 * @brief resumes a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void resume(void) const throw() = 0;

	/**
	 * @brief Holds a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void hold(void) const throw() = 0;

	/**
	 * @brief Releases a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void release(void) const throw() = 0;

	/**
	 * @brief Terminates a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void terminate(void) const throw() = 0;

	/**
	 * @brief Clean up any data about this job
	 *
	 * @param - None
	 *
	 * @throw InvalidArgumentException - Reaped job in any subsequent activity
	 * 									throws an InvalidArgumentException for
	 * 									the job parameter
	 *
	 * @return None
	 */
	virtual void reap(void) const = 0;

	/**
	 * @brief Blocking call to wait until job starts
	 *
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw TimeoutException - Fails to start in a given duration
	 *
	 * @return None
	 */
	virtual void waitStarted(const TimeAmount& timeout_) = 0;

	/**
	 * @brief Blocking call to wait until job ends
	 *
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw TimeoutException - Fails to terminate in a given duration
	 *
	 * @return None
	 */
	virtual void waitTerminated(TimeAmount& timeout_) = 0;
};
typedef list<Job*> JobList;

/**
 * @class JobArray
 * @brief Abstract class represents a set of jobs created by one operation.
 * 			JobArray instances are only created by the runBulkJobs method
 */
class JobArray {
public:
	/**
	 * Destructor
	 */
	virtual ~JobArray(void) {
	}

	/**
	 * @brief Returns Job array ID
	 *
	 * @param - None
	 *
	 * @return valid job array ID
	 */
	virtual const string& getJobArrayId(void) const = 0;

	/**
	 * @brief Returns List of jobs in JobArray
	 *
	 * @param - None
	 *
	 * @return List of Job
	 */
	virtual JobList& getJobs(void) = 0;

	/**
	 * @brief Returns JobTemplate from which JobArray is submitted
	 *
	 * @param - None
	 *
	 * @return JobTemlpate
	 */
	virtual const JobTemplate& getJobTemplate(void) const = 0;

	/**
	 * @brief Suspends a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void suspend(void) const = 0;

	/**
	 * @brief resumes a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void resume(void) const = 0;

	/**
	 * @brief Holds a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void hold(void) const = 0;

	/**
	 * @brief Releases a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void release(void) const = 0;

	/**
	 * @brief Terminates a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void terminate(void) const = 0;

	/**
	 * @brief Clean up any data about this JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidArgumentException - Reaped JobArray in any subsequent activity
	 * 									throws an InvalidArgumentException for
	 * 									the JobArray parameter
	 *
	 * @return None
	 */
	virtual void reap(void) const = 0;
};

/**
 * @class Reservation
 * @brief Abstract class represents attributes and methods available for
 * 			an advance reservation successfully created in the DRMS.
 */

class Reservation {
public:
	/**
	 * Destructor
	 */
	virtual ~Reservation(void) {
	};

	/**
	 * @brief Returns reservation Id given by DRMS
	 *
	 * @param - None
	 *
	 * @return valid reservation Id
	 */
	virtual const string& getReservationId(void) const = 0;

	/**
	 * @brief Returns ReservationTemplate from which Reservation is submitted
	 *
	 * @param - None
	 *
	 * @return JobTemlpate
	 */
	virtual const ReservationTemplate& getReservationTemplate(void) const = 0;

	/**
	 * @brief Returns detailed information of Reservation
	 *
	 * @param - None
	 *
	 * @return ReservationInfo
	 */
	virtual const ReservationInfo& getInfo(void) const = 0;

	/**
	 * @brief Terminate the Reservation
	 *
	 * @param - None
	 *
	 * @return None
	 */
	virtual void terminate(void) const = 0;
};
typedef list<Reservation*> ReservationList;

/**
 * @class ReservationSession
 * @brief Abstract class acts as container for advance reservations in
 * 			the DRMS
 */
class ReservationSession {
private:
	const string contact; /*!< DRMS Contact name */
	const string sessionName; /*!< Session name */
public:

	/**
	 * @brief Constructor
	 */
	ReservationSession() {

	}

	/**
	 * @brief Constructor
	 *
	 * @param contact_ - DRMS contact name
	 * @param sessionName_ - Session Name
	 *
	 */
	ReservationSession(const string& contact_, const string sessionName_) :
			contact(contact_), sessionName(sessionName_) {

	}

	/**
	 * Destructor
	 */
	virtual ~ReservationSession(void) {
	};

	/**
	 * @brief Returns DRMS contact name
	 *
	 * @param - None
	 *
	 * @return DRMS Name
	 */
	virtual const string& getContact(void) const {
		return contact;
	}

	/**
	 * @brief Returns session name
	 *
	 * @param - None
	 *
	 * @return session Name
	 */
	virtual const string& getSessionName(void) const {
		return sessionName;
	}

	/**
	 * @brief Returns Reservation of a given ID
	 *
	 * @param[in] reservationId_ - reservationId_ to be searched in the list
	 *
	 * @return Reservation
	 */
	virtual const Reservation& getReservation(const string& reservationId_) = 0;

	/**
	 * @brief Request DRMS to create a reservation
	 *
	 * @param[in] reservationTemplate_ - reservation information
	 *
	 * @return Reservation
	 */
	virtual const Reservation& requestReservation(
			const ReservationTemplate& reservationTemplate_) const = 0;

	/**
	 * @brief Returns associated list of reservations
	 *
	 * @param - None
	 *
	 * @return ReservationList
	 */

	virtual const ReservationList& getReservations(void) = 0;
};

/**
 * @class JobSession
 * @brief Abstract class acts as container for advance jobs in
 * 			the DRMS
 */
class JobSession {
private:
	const string sessionName; /*!< Session name */
	StringList jobCategories; /*!< List of Categories*/
	const string contact; /*!< DRMS Contact name */
public:

	/**
	 * @brief Constructor
	 */
	JobSession() {

	}

	/**
	 * @brief Constructor
	 *
	 * @param sessionName_ - Session Name
	 * @param jobCategories_ - Job Categories
	 * @param contact_ - DRMS contact name
	 *
	 */
	JobSession( const string& sessionName_,
			const StringList& jobCategories_, 
			const string& contact_) : sessionName(sessionName_),
					jobCategories(jobCategories_) , contact(contact_) {

	}

	/**
	 * Destructor
	 */
	virtual ~JobSession(void){
	}

	/**
	 * @brief Returns DRMS contact name
	 *
	 * @param - None
	 *
	 * @return DRMS Name
	 */
	virtual const string& getContact(void) const {
		return contact;
	}
	/**
	 * @brief Returns session name
	 *
	 * @param - None
	 *
	 * @return session Name
	 */
	virtual const string& getSessionName(void) const {
		return sessionName;
	}
	/**
	 * @brief Returns supported job categories
	 *
	 * @param - None
	 *
	 * @return Job categories list
	 */
	virtual const StringList& getJobCategories(void) const {
		return jobCategories;
	}

	/**
	 * @brief Returns associated jobs
	 *
	 * @param[in] filter_ - Filter criteria
	 *
	 * @return JobList
	 */
	virtual const JobList& getJobs(const JobInfo& filter_) = 0;

	/**
	 * @brief Returns JobArray
	 *
	 * @param[in] jobArrayId_ - JobArray to be searched in the list
	 *
	 * @throw InvalidArgumentException - If the ID is not found
	 *
	 * @return JobArray
	 */
	virtual const JobArray& getJobArray(const string& jobArrayId_) = 0;

	/**
	 * @brief Submit Job to DRMS
	 *
	 * @param[in] jobTemplate_ - Detailed job information
	 *
	 * @throw InvalidArgumentException - If DRMS fails to accept JobTemplate
	 *
	 * @return Reference to Job object from the DRMS
	 */
	virtual const Job& runJob(const JobTemplate& jobTemplate_) const = 0;

	/**
	 * @brief Submit JobArray to DRMS
	 *
	 * @param[in] jobTemplate_ - Detailed job information
	 * @param[in] beginIndex_ - start index of JobArray
	 * @param[in] endIndex_ - end index of JobArray
	 * @param[in] step_ - Incremental count for index
	 * @param[in] maxParallel_ - Number of jobs to run in parallel
	 *
	 * @throw InvalidArgumentException - If DRMS fails to accept JobTemplate
	 *
	 * @return JobArray
	 */
	virtual const JobArray& runBulkJobs(const JobTemplate& jobTemplate_,
			const long beginIndex_, const long endIndex_, const long step_,
			const long maxParallel_) const = 0;

	/**
	 * @brief In a list of specified job ids waits until
	 * 			any of the job is started
	 *
	 * @param[in] jobs_ - List of Jobs
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw InvalidArgumentException - Invalid list of jobs
	 * @throw TimeoutException - Fails to start in a given duration
	 *
	 * @return Job
	 */
	virtual const Job& waitAnyStarted(const JobList& jobs_,
			const TimeAmount timeout_) = 0;

	/**
	 * @brief In a list of specified job ids waits until
	 * 			any of the job is terminated
	 *
	 * @param[in] jobs_ - List of Jobs
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw InvalidArgumentException - Invalid list of jobs
	 * @throw TimeoutException - Fails to terminate in a given duration
	 *
	 * @return Job
	 */
	virtual const Job& waitAnyTerminated(const JobList& jobs_,
			const TimeAmount timeout_) = 0;
};

/**
 * @class MonitoringSession
 * @brief Abstract class provides a set of stateless methods for fetching
 * 			information about the DRMS
 */
class MonitoringSession {
public:

	/**
	 * Destructor
	 */
	virtual ~MonitoringSession(void) {
	};

	/**
	 * @brief Returns list of machines available in the DRM system as
	 * 			execution host.
	 *
	 * @param[in] machines_ - Filter criteria for machines
	 *
	 * @return MachineInfoList
	 */
	virtual const MachineInfoList& getAllMachines(
			list<string> machines_) const = 0;

	/**
	 * @brief Returns list reservations visible for the user running
	 * 			the DRMAA-based application
	 *
	 * @param - None
	 *
	 * @return ReservationList
	 */
	virtual const ReservationList& getAllReservations(void) const = 0;

	/**
	 * @brief Returns list Jobs visible for the user running
	 * 			the DRMAA-based application
	 *
	 * @param[in] filter_ - Filter criteria
	 *
	 * @return JobList
	 */
	virtual const JobList& getAllJobs(JobInfo& filter_) const = 0;

	/**
	 * @brief Returns list of Queues available in the DRM system.
	 *
	 * @param[in] queues_ - Filter criteria for queues
	 *
	 * @return QueueInfoList
	 */
	virtual const QueueInfoList& getAllQueues(list<string> queues_) const = 0;
};

/**
 * @class Singleton
 * @brief Template singleton class to make a Class as Singleton
 * 			IfaceT - Abstract data type class
 * 			ImplT - Concrete class of IfaceT
 */

template<typename IfaceT, typename ImplT>
class Singleton {
private:
	static IfaceT * m_instance;

	/**
	 * @brief Default constructor
	 */
	Singleton();

	/**
	 * @brief Copy constructor is private so that
	 * 			instance cannot be created outside class
	 * 	@param obj_ - Singleton object to be copied
	 */
	Singleton(Singleton const& obj_);
	/**
	 * @brief Assignment operator is overloaded and is private
	 * 			so that instance cannot be created outside class
	 * 	@param obj_ - Singleton object to be copied
	 */
	Singleton& operator=(Singleton const& obj_);

public:
	/**
	 * @brief to get single Instance
	 *
	 * @return  IfaceT - Abstract class reference
	 *
	 */
	static IfaceT* getInstance() {
		pthread_mutex_lock(&ImplT::_posixMutex);
		if (m_instance == NULL) {
			m_instance = new ImplT();
			/*assert(m_instance!= NULL);*/
		}
		pthread_mutex_unlock(&ImplT::_posixMutex);
		return m_instance;
	}
};

template<typename IfaceT, typename ImplT>
IfaceT *Singleton<IfaceT, ImplT>::m_instance = NULL;

/**
 * @class SessionManager
 * @brief Abstract class is used for establishing communication
 * 			with the DRM system. By the help of this interface, sessions for
 * 			job management, monitoring,and/or reservation management can be
 * 			maintained
 */
class SessionManager {

private:
	const string drmsName; /*!< system identifier denoting the DRM system targeted
	 by the implementation*/
	const Version drmsVersion; /*!< provides the DRM-system specific version
	 information.*/
	const string drmaaName; /*!< Contains a string identifying the vendor of
	 the DRMAA implementation.*/
	const Version drmaaVersion; /*!< provides the minor / major version number
	 information for the DRMAA implementation*/

public:
	SessionManager() :
			drmsName(DRMS_NAME), drmsVersion(DRMS_MAJOR_VERSION,
					DRMS_MINOR_VERSION), drmaaName(DRMAA_NAME), drmaaVersion(
			DRMAA_MAJOR_VERSION, DRMAA_MINOR_VERSION) {
	}
	/**
	 * @brief Constructor
	 *
	 * @param drmsName_ - DRMS Name
	 * @param drmaaName_- DRMAA Name
	 * @param drmsVersion_ - DRMS Version
	 * @param drmaaVersion_ - DRMAA Version
	 *
	 */
	SessionManager(string& drmsName_, Version& drmsVersion_, string& drmaaName_,
			Version& drmaaVersion_) :
			drmsName(drmsName_), drmsVersion(drmsVersion_), drmaaName(
					drmaaName_), drmaaVersion(drmaaVersion_) {

	}

	/**
	 * Destructor
	 */
	virtual ~SessionManager(void) {};

	/**
	 * @brief Returns DRMAA implementation vendor name
	 *
	 * @param - None
	 *
	 * @return Name
	 */
	const string& getDrmaaName(void) const {
		return drmaaName;
	}

	/**
	 * @brief Returns DRMAA implementation version
	 *
	 * @param - None
	 *
	 * @return Version
	 */

	const Version& getDrmaaVersion(void) const {
		return drmaaVersion;
	}

	/**
	 * @brief Returns DRMS name
	 *
	 * @param - None
	 *
	 * @return Name
	 */
	const string& getDrmsName(void) const {
		return drmsName;
	}

	/**
	 * @brief Returns DRMS version
	 *
	 * @param - None
	 *
	 * @return Version
	 */
	const Version& getDrmsVersion(void) const {
		return drmsVersion;
	}

	/**
	 * @brief Allows to test if the DRMAA implementation supports a feature
	 * 			specified as optional.The allowed input values are specified in
	 * 			the DrmaaCapability enumeration
	 *
	 * @param[in] capability_ - DrmaaCapability
	 *
	 * @return true if supported
	 * 		  false if not supported
	 */
	virtual bool supports(const DrmaaCapability& capability_) = 0;

	/**
	 * @brief Creates and opens a new job session instance. On successful
	 * 			completion of this method, the necessary initialization for
	 * 			making the session usable.
	 *
	 * @param[in] sessionName_ - Session Name
	 * @param[in] contact_ - DRMS contact name
	 *
	 * @throw InvalidArgumentException - If a session with sessionName_
	 * 										already exists
	 *
	 * @return JobSession
	 *
	 */
	virtual const JobSession& createJobSession(const string& sessionName_,
			const string& contact_) = 0;

	/**
	 * @brief To open a persisted JobSession instance that has previously
	 * 			been created under the given sessionName
	 *
	 * @param[in] sessionName_ - Session Name
	 *
	 * @throw InvalidArgumentException - If a session with sessionName_
	 * 										already exists

	 * @return JobSession
	 *
	 */
	virtual const JobSession& openJobSession(const string& sessionName_) = 0;

	/**
	 * @brief Performs the necessary action to disengage from the DRM system.
	 *
	 * @param[in] session_ - JobSession
	 *
	 * @throw InvalidSessionException - If session doesn't exists
	 *
	 * @return None
	 */
	virtual void closeJobSession(JobSession& session_) = 0;

	/**
	 * @brief Cached state information for the given session name is removed
	 *
	 * @param[in] sessionName_ - Session Name
	 *
	 * @throw InvalidArgumentException - If a session with sessionName_
	 * 										already exists
	 *
	 * @return None
	 */
	virtual void destroyJobSession(const string& sessionName_) = 0;

	/**
	 * @brief Creates and opens a new Reservation session instance. On
	 * 			successful completion of this method, the necessary initialization
	 * 			for making the session usable.
	 *
	 * @param[in] sessionName_ - Session Name
	 * @param[in] contact_ - DRMS contact name
	 *
	 * @return ReservationSession
	 *
	 */
	virtual const ReservationSession& createReservationSession(
			const string& sessionName_, const string& contact_) = 0;

	/**
	 * @brief To open a persisted ReservationSession instance that has previously
	 * 			been created under the given sessionName
	 *
	 * @param[in] sessionName_ - Session Name
	 *
	 * @throw UnsupportedOperationException - Not applicable
	 * @throw InvalidArgumentException - If a session with sessionName_
	 * 										already exists
	 *
	 * @return ReservationSession
	 *
	 */
	virtual const ReservationSession& openReservationSession(
			const string& sessionName_) = 0;

	/**
	 * @brief Performs the necessary action to disengage from the DRM system.
	 *
	 * @param[in] session_ - ReservationSession
	 *
	 * @throw UnsupportedOperationException - Not applicable
	 * @throw InvalidSessionException - If session doesn't exists
	 *
	 * @return None
	 */
	virtual void closeReservationSession(ReservationSession& session_) = 0;

	/**
	 * @brief Cached state information for the given session name is removed
	 *
	 * @param[in] sessionName_ - Session Name
	 *
	 * @throw UnsupportedOperationException - Not applicable
	 * @throw InvalidArgumentException - If a session with sessionName_
	 * 										already exists
	 *
	 * @return None
	 */
	virtual void destroyReservationSession(const string& sessionName_) = 0;

	/**
	 * @brief Opens a stateless single instance for fetching information about
	 * 			 the DRM system. On successful completion of this method, the
	 * 			 necessary initialization for making the session usable
	 *
	 * @param[in] contact_ - DRMS contact name
	 *
	 * @return MonitoringSession
	 *
	 */
	virtual const MonitoringSession& openMonitoringSession(
			const string& contact_) = 0;

	/**
	 * @brief Performs the necessary action to disengage from the DRM system.
	 *
	 * @param[in] session_ - MonitoringSession
	 *
	 * @throw InvalidSessionException - If session doesn't exists
	 *
	 * @return None
	 */
	virtual void closeMonitoringSession(const MonitoringSession& session_) = 0;

	/**
	 * @brief Returns a list of Reservation names that are valid input for
	 * 			the openReservation method.

	 * @param - None
	 *
	 * @return List of ReservatioSession name
	 *
	 */
	virtual const StringList& getReservationSessionNames(void) = 0;

	/**
	 * @brief Returns a list of JobSession names that are valid input for
	 * 			the openJobSession method.

	 * @param - None
	 *
	 * @return List of JobSession name
	 *
	 */
	virtual const StringList& getJobSessionNames(void) = 0;

	/**
	 * @brief registers for event from DRMS

	 * @param callback - call back details
	 *
	 * @throw UnsupportedOperationException - If the functionality is not
	 * 											supported
	 * @return None
	 *
	 */
	virtual void registerEventNotification(const DrmaaCallback& callback) = 0;

	/**
	 * @brief Initializes SessionManager
	 *
	 * @throw InternalException - If the init fails
	 *
	 * @return None
	 *
	 */
	virtual void initialize() = 0;

};
}
#endif /* DRMAA2_HPP_ */
