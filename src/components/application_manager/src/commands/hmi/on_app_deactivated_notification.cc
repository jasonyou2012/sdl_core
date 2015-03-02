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

#include "application_manager/commands/hmi/on_app_deactivated_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

OnAppDeactivatedNotification::OnAppDeactivatedNotification(
    const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnAppDeactivatedNotification::~OnAppDeactivatedNotification() {
}

void OnAppDeactivatedNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t app_id = (*message_)[strings::msg_params][strings::app_id].asUInt();
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id);
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application not found, id="<<app_id);
    return;
  }

  using namespace mobile_apis::HMILevel;
  using namespace helpers;
  if (!(((hmi_apis::Common_DeactivateReason::AUDIO ==
      (*message_)[strings::msg_params][hmi_request::reason].asInt()) ||
      (hmi_apis::Common_DeactivateReason::PHONECALL ==
            (*message_)[strings::msg_params][hmi_request::reason].asInt())) &&
      (app->hmi_level() == HMI_LIMITED))) {
    app = ApplicationManagerImpl::instance()->active_application();
    if (!app.valid()) {
      LOG4CXX_ERROR_EXT(logger_, "OnAppDeactivatedNotification no active app!");
      return;
    }
    if (app_id != app->app_id()) {
      LOG4CXX_ERROR_EXT(logger_, "Wrong application id!");
      return;
    }
  }

  if (HMI_NONE == app->hmi_level()) {
    return;
  }

  if (Common_DeactivateReason::AUDIO == deactivate_reason) {
    if (app->is_media_application() && !(app->is_navi())) {
      if (profile::Profile::instance()->is_mixing_audio_supported() &&
          (ApplicationManagerImpl::instance()->vr_session_started() ||
          app->tts_speak_state())) {
        app->set_audio_streaming_state(AudioStreamingState::ATTENUATED);
      } else {
        new_hmi_level = HMI_LIMITED;
      }
      ApplicationManagerImpl::instance()->
          ChangeAppsHMILevel(app->app_id(), HMILevel::HMI_BACKGROUND);
      MessageHelper::SendHMIStatusNotification(*app);
      return;
    }
  }

  if (new_hmi_level != app->hmi_level()) {
    ApplicationManagerImpl::instance()->ChangeAppsHMILevel(app->app_id(),
                                                           new_hmi_level);
    MessageHelper::SendHMIStatusNotification(*app);
  }
}

}  // namespace commands

}  // namespace application_manager

