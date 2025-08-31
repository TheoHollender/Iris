
/**
 * MIT License
 * 
 * Copyright (c) Théo Hollender, 2025
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file contains the list of defines used by Project Iris in its files.
 * 
 * IRIS_DISABLE - All the provided software gets disabled.
 *   In particular all the Iris macros will be defined to empty,
 *   Leaving no time cost to the program running.
 * 
 * IRIS_DEBUG - The software provided will start logging itself.
 *   This is mainly a way for project developpers to understand what
 *   Is going wrong when working with core aspects of the program.
 * 
 * IRIS_CLI - The software provided will stop the execution of the program
 *   during iris_init() to work as a Command Line Interface. Whenever iris_init()
 *   is called, it is expected for the types used by iris to have been correctly initialized.
 * 
 * Severity Levels for Logs.
 * 
 * IRIS_FATAL - Display logs for the FATAL severity and above.
 * IRIS_ERROR - Display logs for the ERROR severity and above.
 * IRIS_WARN  - Display logs for the WARN  severity and above.
 * IRIS_INFO  - Display logs for the INFO  severity and above.
 * IRIS_DEBUG - Display logs for the DEBUG severity and above.
 * 
 * In case none of the severity is enabled, IRIS_ERROR will be
 * automatically enabled.
 */

#ifdef IRIS_DISABLE
#undef IRIS_DEBUG
#undef IRIS_CLI
#endif
