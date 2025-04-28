/**
 * I2C1 Generated Driver API Header File
 *
 * @file i2c1.h
 *
 * @defgroup i2c_host I2C1_HOST
 * @brief This header file contains API prototypes and other data types for the I2C1 driver.
 *
 * @version I2C1 Driver Version 2.2.0 \n
 * 
 * @version I2C1 Package Version 6.2.0
 */

/*
� [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef I2C1_H
#define I2C1_H

/**
 * @misradeviation {advisory,2.5} False positive - A few macros in this file
 * are used as arguments but are not getting recognized by the tool.
 * This rule ID has been disabled at the project level due to numerous
 * instances across various files.
 * Consequently, in the application project, this rule ID must be disabled
 * in the MPLAB-X IDE by navigation to Tools -> Options -> Embedded -> MISRA Check.
*/

#include <stdint.h>
#include <stdbool.h>
#include "i2c_host_event_types.h"
#include "i2c_host_interface.h"


#define i2c1_host_host_interface I2C1_Host


#define I2C1_Host_Initialize I2C1_Initialize
#define I2C1_Host_Deinitialize I2C1_Deinitialize
#define I2C1_Host_Write I2C1_Write
#define I2C1_Host_Read I2C1_Read
#define I2C1_Host_WriteRead I2C1_WriteRead
#define I2C1_Host_ErrorGet I2C1_ErrorGet
#define I2C1_Host_CallbackRegister I2C1_CallbackRegister
#define I2C1_Host_IsBusy I2C1_IsBusy

#define I2C1_Host_Tasks I2C1_Tasks



/**
 * @ingroup I2C_HOST
 * @brief External object for I2C1_Host.
 */
extern const i2c_host_interface_t I2C1_Host;

/**
 * @ingroup i2c_host
 * @brief Initializes the I2C1 module based on the configurable options in the MPLAB® Code Configurator (MCC) Melody UI.
 * @param None.
 * @return None. \n
 */
void I2C1_Initialize(void);

/**
 * @ingroup i2c_host
 * @brief Sets the registers to Power-on Reset (POR) values.
 * @param None.
 * @return None. \n
 */
void I2C1_Deinitialize(void);

/**
 * @ingroup i2c_host
 * @brief Writes data to a client on the bus.
 *        The function will attempt to write the length number of bytes from the data
 *        buffer to a client whose address is specified by the address argument.
 *
 *        The I2C host will generate a Start condition, write the data and then
 *        generate a Stop condition. If the client NAKs the request or a bus 
 *        error was encountered on the bus, the transfer is terminated.
 *        The application can call the I2C1_ErrorGet() function to
 *        check the cause of the error. The function is non-blocking, initiating the 
 *        bus activity and returning immediately. The transfer is then completed in the peripheral 
 *        interrupt. For the Polling mode, the user has to call  I2C1_Tasks
 *        in the while loop. A transfer request cannot be placed when another 
 *        transfer is in progress. Calling this function when another function 
 *        is already in progress will cause the function to return false.
 
 * @param [in] address - 7-bit/10-bit client address
 * @param [in] *data - Pointer to the source data buffer containing the data to be transmitted
 * @param [in] dataLength - Length of data buffer in number of bytes as well as the number of bytes to be written
 * @return True - The request was placed successfully and the bus activity was initiated \n
 * @return False - The request failed because there was already a transfer in progress when this function was called \n
 */
bool I2C1_Write(uint16_t address, uint8_t *data, size_t dataLength);

/**
 * @ingroup i2c_host
 * @brief Reads data to a client on the bus.
 *        The function will attempt to read the length number of bytes into the data
 *        buffer from a client whose address is specified by the address argument. 
 * 
 *        The I2C host generates a Start condition, reads the data and then 
 *        generates a Stop condition. If the client NAKs the request or a bus 
 *        error is encountered on the bus, the transfer is terminated. 
 *        The application will call the I2C1_ErrorGet() function to check
 *         the cause of the error. The function is non-blocking, initiating the 
 *        bus activity and returning immediately. The transfer is then completed in the peripheral 
 *        interrupt. For the Polling mode, the user has to call  I2C1_Tasks
 *        in the while loop. A transfer request cannot be placed when another 
 *        transfer is in progress. Calling this function when another function 
 *        is already in progress will cause the function to return false.
 * @param [in] address - 7-bit/10-bit client address
 * @param [out] *data - Pointer to the destination data buffer containing the data to be received
 * @param [in] dataLength - Length of data buffer in number of bytes as well as the number of bytes to be read
 * @return True - The request was placed successfully and the bus activity was initiated \n
 * @return False - The request failed because there was already a transfer in progress when this function was called \n
 */
bool I2C1_Read(uint16_t address, uint8_t *data, size_t dataLength);

/**
 * @ingroup i2c_host
 * @brief Writes data from the writeData to the bus and then reads data from the client and stores the
 *        received information in the readData. The function generates a Start condition on the bus, 
 *        then sends the writeLength number of bytes contained in the writeData. 
 *        The function will insert a Repeated Start condition and proceed to read the
 *        readLength number of bytes from the client. The received bytes are stored
 *        in the readData buffer. A Stop condition is generated after the last byte has been received.
 *        
 *        If the client NAKs the request or a bus error was encountered on 
 *        the bus, the transfer is terminated. The application can call the
 *        I2C1_ErrorGet() function to check the cause of the error.
 *
 *        The function is non-blocking, initiating the bus activity and returning
 *        immediately. The transfer is then completed in the peripheral 
 *        interrupt. For Polling mode, the user has to call  I2C1_Tasks
 *        in while loop. A transfer request cannot be placed when another transfer 
 *        is in progress. Calling this function when another transfer is already 
 *        in progress will cause the function to return false.
 * @param [in] address - 7-bit/10-bit client address
 * @param [in] *writeData   - Pointer to write data buffer
 * @param [in] writeLength - Write data length in bytes
 * @param [out] *readData - Pointer to read data buffer
 * @param [in] readLength - Read data length in bytes
 *@return True - The request was placed successfully and the bus activity was initiated \n
 *@return False - The request failed because there was already a transfer in progress when this function was called \n
 */
bool I2C1_WriteRead(uint16_t address, uint8_t *writeData, size_t writeLength, uint8_t *readData, size_t readLength);

/**
 * @ingroup i2c_host
 * @brief Returns the error that occurred during the last I2C transmit and receive event.    
 * @param None.
 * @return I2C_ERROR_NONE - No error \n
 * @return I2C_ERROR_NACK - Client returned NACK \n
 * @return I2C_ERROR_BUS_COLLISION - Bus collision \n
 */
i2c_host_error_t I2C1_ErrorGet(void);

/**
 * @ingroup i2c_host
 * @brief Checks if I2C is busy. 
 *        Initialize the I2C peripheral with @ref I2C1_Initialize() before calling this API.
 * @param None.
 * @return True - I2C is busy \n
 * @return False - I2C is free \n
 */
bool I2C1_IsBusy(void);

/**
 * @ingroup i2c_host
 * @brief Setter function for the I2C callback. This routine is called when an error is generated.
 * @param callback - Pointer to the custom callback
 * @return None. \n
 *
 */
void I2C1_CallbackRegister(void (*callback)(void));

/**
 * @ingroup i2c_host
 * @brief Handles I2C tasks when the peripheral interrupts are disabled. This function must be called in application.
 * @param None.
 * @return None. \n
 */
void I2C1_Tasks(void);

#endif /* I2C1_H */