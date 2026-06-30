#ifndef OPENVELA_LOCAL_COMPAT_H
#define OPENVELA_LOCAL_COMPAT_H

/* VelaBridge local build-only compatibility header.
 * This records missing HAL callback defaults while diagnosing the
 * sf32lb52_lchspi_ulp openvela build. Do not submit upstream.
 */

/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */
#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL
#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL
typedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;
#endif

#ifndef USE_HAL_COMP_REGISTER_CALLBACKS
#define USE_HAL_COMP_REGISTER_CALLBACKS 0U
#endif
#ifndef USE_HAL_I2C_REGISTER_CALLBACKS
#define USE_HAL_I2C_REGISTER_CALLBACKS 0U
#endif
#ifndef USE_HAL_UART_REGISTER_CALLBACKS
#define USE_HAL_UART_REGISTER_CALLBACKS 0U
#endif
#ifndef USE_HAL_RNG_REGISTER_CALLBACKS
#define USE_HAL_RNG_REGISTER_CALLBACKS 0U
#endif
#ifndef USE_HAL_HCD_REGISTER_CALLBACKS
#define USE_HAL_HCD_REGISTER_CALLBACKS 0U
#endif

#endif /* OPENVELA_LOCAL_COMPAT_H */