#ifndef OPENVELA_LOCAL_COMPAT_H
#define OPENVELA_LOCAL_COMPAT_H

/* VelaBridge local build-only compatibility header.
 * This records missing HAL callback defaults while diagnosing the
 * sf32lb52_lchspi_ulp openvela build. Do not submit upstream.
 */

#ifndef USE_HAL_COMP_REGISTER_CALLBACKS
#define USE_HAL_COMP_REGISTER_CALLBACKS 0U
`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif
#ifndef USE_HAL_I2C_REGISTER_CALLBACKS
#define USE_HAL_I2C_REGISTER_CALLBACKS 0U
`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif
#ifndef USE_HAL_UART_REGISTER_CALLBACKS
#define USE_HAL_UART_REGISTER_CALLBACKS 0U
`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif
#ifndef USE_HAL_RNG_REGISTER_CALLBACKS
#define USE_HAL_RNG_REGISTER_CALLBACKS 0U
`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif
#ifndef USE_HAL_HCD_REGISTER_CALLBACKS
#define USE_HAL_HCD_REGISTER_CALLBACKS 0U
`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif

`n/* VelaBridge local workaround: break HAL DMA circular include for pointer declarations. */`n#ifndef OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`n#define OPENVELA_LOCAL_DMA_HANDLE_FORWARD_DECL`ntypedef struct __DMA_HandleTypeDef DMA_HandleTypeDef;`n#endif`n
#endif /* OPENVELA_LOCAL_COMPAT_H */
