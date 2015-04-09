/*
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_JSON_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_JSON_H_

#include "application_manager/resumption/resumption_data.h"
#include "json/json.h"

namespace resumption {

class ResumptionDataJson : public ResumptionData {
  public:

    ResumptionDataJson();

  /**
   * @brief Save application persistent info for future resuming on json format
   * @param application is application witch need to be saved
   */
    virtual void SaveApplication(app_mngr::ApplicationConstSharedPtr application);

  /**
   * @brief Returns HMI level of application from saved data
   * @param m_app_id contains mobile application id of application
   * @param device_id contains device id
   * @return HMI level if saved data does not contain HMI level method
   * returns -1
   */
  virtual int GetStoredHMILevel(const std::string& m_app_id,
                                 const std::string& device_id);

  /**
   * @brief Check if saved data of applications have hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, false otherwise
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id);

  /**
   * @brief Checks if saved data have application
   * and removes this data if it is not valid
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data exists and data is valid, false otherwise
   */
  virtual bool CheckSavedApplication(const std::string& mobile_app_id,
                                     const std::string& device_id);

  /**
   * @brief Retrieves HMI app ID for the given mobile app ID
   * and device ID from stored information.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& mobile_app_id,
                                       const std::string& device_id);

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnSuspend();

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnAwake();

  /**
   * @brief Retrieves hash ID for the given mobile app ID
   * and device ID from stored information.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - parameter which will contain HASH id from saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetHashId(const std::string& mobile_app_id,
                         const std::string& device_id,
                         std::string& hash_id);

  /**
   * @brief Retrieves data of saved appliction for the given mobile app ID
   * and device ID
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param saved_app - parameter which will contain data of saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetSavedApplication(const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   smart_objects::SmartObject& saved_app);

  /**
   * @brief Remove application from list of saved applications
   * @param mobile_app_id application witch need to be removed
   * @param device_id - contains id of device on which is running application
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(const std::string& mobile_app_id,
                                  const std::string& device_id);

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  virtual uint32_t GetIgnOffTime();

  /**
   * @brief Checks if saved data have application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return index if data of application exists, otherwise returns -1
   */
  virtual int IsApplicationSaved(const std::string& mobile_app_id,
                                 const std::string& device_id);

  /**
   * @brief Retrieves data from saved application
   * @param  will be contain data for resume_ctrl
   */
  virtual void GetDataForLoadResumeData(smart_objects::SmartObject& saved_data);

  /**
   * @brief Updates HMI level of saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains hmi level for saved appliction
   */
  virtual void UpdateHmiLevel(const std::string& mobile_app_id,
                              const std::string& device_id,
                              int32_t hmi_level);

  virtual ~ResumptionDataJson();

  virtual bool Init();



  private:

  /**
   * @brief GetFromSavedOrAppend allows to get existed record about application
   * or adds the new one.
   * @param mobile_app_id application id.
   * @param device_id unique id of device.
   * @return the reference to the record in applications array.
   */
  Json::Value& GetFromSavedOrAppend(const std::string& mobile_app_id,
                                    const std::string& device_id);

  /**
   * @brief Get applications for resumption of LastState
   * @return applications for resumption of LastState
   */
  Json::Value& GetSavedApplications();

  /**
   * @brief Get Resumption section of LastState
   * @return Resumption section of LastState in Json
   */
  Json::Value& GetResumptionData();

  /**
   * @brief GetObjectIndex allows to obtain specified object index from
   * applications arrays.
   * @param mobile_app_id application id that should be found.
   * @param device_id unique id of device.
   * @return application's index of or -1 if it doesn't exists
   */
  int GetObjectIndex(const std::string& mobile_app_id,
                     const std::string& device_id);

  /**
   * @brief Return true if application resumption data is valid,
   * otherwise false
   * @param index application index in the resumption list
   */
  bool IsResumptionJSONDataValid(uint32_t index);

  /**
   * @brief Set applications for resumption to LastState
   * @parems apps_json applications to write in LastState
   */
  void SetSavedApplication(Json::Value& apps_json);

  /**
   * @brief Setup IgnOff time to LastState
   * @param ign_off_time - igition off time
   */
  void SetLastIgnOffTime(time_t ign_off_time);

  /*
   * @brief Return true if application resumption data is valid,
   * otherwise false
   *
   * @param index application index in the resumption list
   */
  bool IsResumptionDataValid(uint32_t index);

  DISALLOW_COPY_AND_ASSIGN(ResumptionDataJson);

};
}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_JSON_H_