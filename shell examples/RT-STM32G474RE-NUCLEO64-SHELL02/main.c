/*
    NeaPolis Innovation Summer Campus Examples
    Copyright (C) 2020-2023 Salvatore Dello Iacono [delloiaconos@gmail.com]
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * [NISC2022-SHELL02] - Project-Local custom shell.
 * DESCRIPTION: The shell has been imported into the project to be modified.
 */

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include <string.h>

static void cmd_hello(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  chprintf(chp, "Hello World!\r\n");
}

static void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {

  static uint8_t led_status = 0;

  if( (argc == 1 && strcmp( argv[0], "ON" ) == 0) ||
      (argc==0 && led_status==0)) {
    palSetLine( LINE_LED_GREEN );
    led_status = 1;
  } else if( (argc == 1 && strcmp( argv[0], "OFF" ) == 0 )||
      (argc==0 && led_status==1)) {
    palClearLine( LINE_LED_GREEN );
    led_status = 0;
  }
    else {
    chprintf(chp, "Usage: led [ON|OFF]\r\n");
  }
}

static const ShellCommand commands[] = {
  {"hello", cmd_hello},
  {"led", cmd_led},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

#define WA_SHELL   2048
THD_WORKING_AREA( waShell, WA_SHELL );


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Initializes a SERIAL driver.
   */
  palSetPadMode( GPIOA, 2, PAL_MODE_ALTERNATE(7) );
  palSetPadMode( GPIOA, 3, PAL_MODE_ALTERNATE(7) );

  sdStart(&SD2, NULL);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Create Shell Thread!
   */
  chThdCreateStatic(waShell, sizeof(waShell), NORMALPRIO, shellThread, (void *)&shell_cfg1);

  while (true) {
    chThdSleepMilliseconds(1000);
  }
}
