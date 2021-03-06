/* vim: set et sw=3 tw=0 fo=croqlaw cino=t0:
 *
 * Astxx, the Asterisk C++ API and Utility Library.
 * Copyright (C) 2005-2007  Matthew A. Nicholson
 * Copyright (C) 2005-2007  Digium, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** @file
 *
 * Include this file to use the command action.  It will be included 
 * automatically by including astxx/manager.h.
 */

#ifndef ASTXX_MANAGER_ACTION_COMMAND_H
#define ASTXX_MANAGER_ACTION_COMMAND_H

#include <astxx/manager/basic_action.h>
#include <astxx/manager/message.h>
#include <astxx/manager/error.h>

namespace astxx {
   namespace manager {
      namespace action {
         /** Execute an Asterisk CLI command.
          * This action is some what of a hack.  If possible use manager 
          * actions instead of CLI commands.
          */
         class command : public basic_action {
            public:
               /** Execute an Asterisk CLI command.
                * @param command the command to execute
                */
               command(const std::string& command) : m_command(command) {
               }
         
               message::action action() const {
                  message::action action("Command");
                  action["Command"] = m_command;
                  return action;
               }

               /** Handle the response from Asterisk.
                * @param response the response
                * @return the response
                */
               message::response handle_response(message::response response) {
                  basic_action::handle_response(response);
                  m_result = response.data;
                  return response;
               }

               /** Get the result of the command.
                * @return the result of the command
                */
               std::string result() {
                  return m_result;
               }

            private:
               std::string m_command;
               std::string m_result;
         };
      }
   }
}

#endif
