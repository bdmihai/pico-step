/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2020 Mihai Baneu                                             |
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
 |  Author: Mihai Baneu                           Last modified: 15.May.2020  |
 |                                                                            |
 |___________________________________________________________________________*/

Product {
    name: 'application'
    type: 'app'

    Depends { name: 'rp' }
    Depends { name: 'boot' }
    Depends { name: 'rp2040' }
    Depends { name: 'freertos' }

    Depends { name: 'pico_runtime' }
    Depends { name: 'pico_sync' }
    Depends { name: 'pico_util' }
    Depends { name: 'pico_time' }
    Depends { name: 'pico_bootrom' }
    Depends { name: 'pico_malloc' }
    Depends { name: 'pico_mem_ops' }
    Depends { name: 'pico_printf' }
    Depends { name: 'pico_bit_ops' }
    Depends { name: 'pico_int64_ops' }
    Depends { name: 'pico_divider' }
    Depends { name: 'pico_float' }
    Depends { name: 'pico_double' }
    Depends { name: 'pico_unique_id' }
    Depends { name: 'pico_binary_info' }
    Depends { name: 'pico_bootsel_via_double_reset' }
    Depends { name: 'pico_stdlib' }
    Depends { name: 'pico_stdio' }
    Depends { name: 'pico_stdio_semihosting' }
    Depends { name: 'pico_stdio_uart' }
    Depends { name: 'pico_multicore' }
    Depends { name: 'hardware_gpio' }
    Depends { name: 'hardware_sync' }
    Depends { name: 'hardware_irq' }
    Depends { name: 'hardware_claim' }
    Depends { name: 'hardware_timer' }
    Depends { name: 'hardware_clocks' }
    Depends { name: 'hardware_watchdog' }
    Depends { name: 'hardware_xosc' }
    Depends { name: 'hardware_pll' }
    Depends { name: 'hardware_divider' } 
    Depends { name: 'hardware_flash' } 
    Depends { name: 'hardware_uart' }
    Depends { name: 'hardware_dma' }
    Depends { name: 'hardware_resets' }
    Depends { name: 'hardware_interp'}
    Depends { name: 'hardware_rtc' }
    Depends { name: 'hardware_adc' }
    Depends { name: 'hardware_vreg' }


    files: [
        '*.h',
        '*.c',
        '*.cpp'
    ]

    Group {
        qbs.install: true
        fileTagsFilter: ['app', 'map', 'bin', 'uf2']
    }
}
