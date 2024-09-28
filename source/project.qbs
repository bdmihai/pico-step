/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2022 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 16.Dec.2022  |
 |                                                                            |
 |___________________________________________________________________________*/
  
import qbs

Project {
    name: 'pico-step'
    minimumQbsVersion: '1.22'
    qbsSearchPaths: '../qbs'

    references: [
        'app/app.qbs',
        'boot/boot.qbs',
        'cmsis/cmsis.qbs',
        'rp2040/rp2040.qbs',
        'tinyusb/tinyusb.qbs',
        //'mbedtls/mbedtls.qbs',
        'freertos/freertos.qbs'
    ]

    SubProject {
        filePath: 'sdk/sdk.qbs'
        Properties {
            projectDefines: [
                //'PICO_TIME_DEFAULT_ALARM_POOL_DISABLED=1'
                //'PICO_STDIO_USB=1'
            ]
        }
    }
}


