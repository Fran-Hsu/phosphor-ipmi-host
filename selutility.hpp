#pragma once

#include <chrono>
#include <cstdint>
#include <ipmid/types.hpp>
#include <sdbusplus/server.hpp>

namespace ipmi
{

namespace sel
{

static constexpr auto mapperBusName = "xyz.openbmc_project.ObjectMapper";
static constexpr auto mapperObjPath = "/xyz/openbmc_project/object_mapper";
static constexpr auto mapperIntf = "xyz.openbmc_project.ObjectMapper";

static constexpr auto logBasePath = "/xyz/openbmc_project/logging/entry";
static constexpr auto logEntryIntf = "xyz.openbmc_project.Logging.Entry";
static constexpr auto logDeleteIntf = "xyz.openbmc_project.Object.Delete";

static constexpr auto propIntf = "org.freedesktop.DBus.Properties";

using ObjectPaths = std::vector<std::string>;
using PropertyName = std::string;
using Resolved = bool;
using Id = uint32_t;
using Timestamp = uint64_t;
using Message = std::string;
using AdditionalData = std::vector<std::string>;
using PropertyType = sdbusplus::message::variant<Resolved, Id, Timestamp,
                                                 Message, AdditionalData>;

// ID string generated using journalctl to include in the MESSAGE_ID field for
// SEL entries.  Helps with filtering SEL entries in the journal.
static constexpr const char* selMessageId = "b370836ccf2f4850ac5bee185b77893a";
static constexpr uint8_t selOperationSupport = 0x02;
static constexpr uint8_t systemEvent = 0x02;
static constexpr size_t systemEventSize = 3;
static constexpr uint8_t oemTsEventFirst = 0xC0;
static constexpr uint8_t oemTsEventLast = 0xDF;
static constexpr size_t oemTsEventSize = 9;
static constexpr uint8_t oemEventFirst = 0xE0;
static constexpr uint8_t oemEventLast = 0xFF;
static constexpr size_t oemEventSize = 13;
static constexpr uint8_t eventMsgRev = 0x04;

static constexpr auto selVersion = 0x51;
static constexpr auto invalidTimeStamp = 0xFFFFFFFF;
static constexpr auto operationSupport = 0x0A;
constexpr static const uint8_t deassertionEvent = 0x80;

/** @struct GetSELInfoResponse
 *
 *  IPMI payload for Get SEL Info command response.
 */
struct GetSELInfoResponse
{
    uint8_t selVersion;       //!< SEL revision.
    uint16_t entries;         //!< Number of log entries in SEL.
    uint16_t freeSpace;       //!< Free Space in bytes.
    uint32_t addTimeStamp;    //!< Most recent addition timestamp.
    uint32_t eraseTimeStamp;  //!< Most recent erase timestamp.
    uint8_t operationSupport; //!< Operation support.
} __attribute__((packed));

static constexpr auto firstEntry = 0x0000;
static constexpr auto lastEntry = 0xFFFF;
static constexpr auto entireRecord = 0xFF;
static constexpr auto selRecordSize = 16;

/** @struct GetSELEntryRequest
 *
 *  IPMI payload for Get SEL Entry command request.
 */
struct GetSELEntryRequest
{
    uint16_t reservationID; //!< Reservation ID.
    uint16_t selRecordID;   //!< SEL Record ID.
    uint8_t offset;         //!< Offset into record.
    uint8_t readLength;     //!< Bytes to read.
} __attribute__((packed));

/** @struct GetSELEntryResponse
 *
 *  IPMI payload for Get SEL Entry command response.
 */
struct GetSELEntryResponse
{
    uint16_t nextRecordID;    //!< Next RecordID.
    uint16_t recordID;        //!< Record ID.
    uint8_t recordType;       //!< Record Type.
    uint32_t timeStamp;       //!< Timestamp.
    uint16_t generatorID;     //!< Generator ID.
    uint8_t eventMsgRevision; //!< Event Message Revision.
    uint8_t sensorType;       //!< Sensor Type.
    uint8_t sensorNum;        //!< Sensor Number.
    uint8_t eventType;        //!< Event Dir | Event Type.
    uint8_t eventData1;       //!< Event Data 1.
    uint8_t eventData2;       //!< Event Data 2.
    uint8_t eventData3;       //!< Event Data 3.
} __attribute__((packed));

/** @struct GetSELEntryResponseOEMTimestamped
 *
 *  IPMI payload for Get SEL Entry command response for OEM Timestamped types.
 */
struct GetSELEntryResponseOEMTimestamped
{
    uint16_t nextRecordID;
    uint16_t recordID;
    uint8_t recordType;
    uint32_t timestamp;
    uint8_t eventData[ipmi::sel::oemTsEventSize];
} __attribute__((packed));

/** @struct GetSELEntryResponseOEM
 *
 *  IPMI payload for Get SEL Entry command response for OEM types.
 */
struct GetSELEntryResponseOEM
{
    uint16_t nextRecordID;
    uint16_t recordID;
    uint8_t recordType;
    uint8_t eventData[ipmi::sel::oemEventSize];
} __attribute__((packed));

/** @struct AddSELEntryRequest
 *
 *  IPMI payload for Add SEL Entry command request.
 */
struct AddSELEntryRequest
{
    uint16_t recordID;
    uint8_t recordType;
    uint32_t timestamp;
    uint16_t generatorID;
    uint8_t eventMsgRevision;
    uint8_t sensorType;
    uint8_t sensorNum;
    uint8_t eventType;
    uint8_t eventData[ipmi::sel::systemEventSize];
} __attribute__((packed));

/** @struct AddSELEntryRequestOEMTimestamped
 *
 *  IPMI payload for Add SEL Entry command request for OEM Timestamped types.
 */
struct AddSELEntryRequestOEMTimestamped
{
    uint16_t recordID;
    uint8_t recordType;
    uint32_t timestamp;
    uint8_t eventData[ipmi::sel::oemTsEventSize];
} __attribute__((packed));

/** @struct AddSELEntryRequestOEM
 *
 *  IPMI payload for Add SEL Entry command request for OEM types.
 */
struct AddSELEntryRequestOEM
{
    uint16_t recordID;
    uint8_t recordType;
    uint8_t eventData[ipmi::sel::oemEventSize];
} __attribute__((packed));

/** @struct DeleteSELEntryRequest
 *
 *  IPMI payload for Delete SEL Entry command request.
 */
struct DeleteSELEntryRequest
{
    uint16_t reservationID; //!< Reservation ID.
    uint16_t selRecordID;   //!< SEL Record ID.
} __attribute__((packed));

static constexpr auto initiateErase = 0xAA;
static constexpr auto getEraseStatus = 0x00;
static constexpr auto eraseComplete = 0x01;

/** @struct ClearSELRequest
 *
 *  IPMI payload for Clear SEL command request.
 */
struct ClearSELRequest
{
    uint16_t reservationID; //!< Reservation ID.
    uint8_t charC;          //!< Char 'C'(0x43h).
    uint8_t charL;          //!< Char 'L'(0x4Ch).
    uint8_t charR;          //!< Char 'R'(0x52h).
    uint8_t eraseOperation; //!< Erase operation.
} __attribute__((packed));

/** @brief Convert logging entry to SEL
 *
 *  @param[in] objPath - DBUS object path of the logging entry.
 *
 *  @return On success return the response of Get SEL entry command.
 */
GetSELEntryResponse convertLogEntrytoSEL(const std::string& objPath);

/** @brief Get the timestamp of the log entry
 *
 *  @param[in] objPath - DBUS object path of the logging entry.
 *
 *  @return On success return the timestamp of the log entry as number of
 *          seconds from epoch.
 */
std::chrono::seconds getEntryTimeStamp(const std::string& objPath);

/** @brief Read the logging entry object paths
 *
 *  This API would read the logging dbus logging entry object paths and sorting
 *  the filename in the numeric order. The paths is cleared before populating
 *  the object paths.
 *
 *  @param[in,out] paths - sorted list of logging entry object paths.
 *
 *  @note This function is invoked when the Get SEL Info command or the Delete
 *        SEL entry command is invoked. The Get SEL Entry command is preceded
 *        typically by Get SEL Info command, so readLoggingObjectPaths is not
 *        invoked before each Get SEL entry command.
 */
void readLoggingObjectPaths(ObjectPaths& paths);

namespace internal
{

/** @brief Convert logging entry to SEL event record
 *
 *  @param[in] objPath - DBUS object path of the logging entry.
 *  @param[in] iter - Iterator to the sensor data corresponding to the logging
 *                    entry
 *
 *  @return On success return the SEL event record, throw an exception in case
 *          of failure.
 */
GetSELEntryResponse
    prepareSELEntry(const std::string& objPath,
                    ipmi::sensor::InvObjectIDMap::const_iterator iter);

} // namespace internal

} // namespace sel

} // namespace ipmi
