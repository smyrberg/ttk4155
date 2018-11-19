/*!
 * @file
 * @brief Macros for bit manipulation.
 */
#ifndef BIT_MANIPULATION_H_
#define BIT_MANIPULATION_H_

#define set_bit(reg, bit) (reg |= (1 << bit))						/*!< Set bit in register high*/
#define clear_bit(reg, bit) (reg &= ~(1 << bit))					/*!< Set bit in register low*/
#define test_bit(reg, bit) (reg & (1 << bit))						/*!< Test if bit in register is set*/
#define spin_wait_for_set(reg, bit) while(!test_bit(reg, bit))		/*!< Busy-Wait for bit to be set*/
#define spin_wait_for_clear(reg, bit) while(test_bit(reg, bit))		/*!< Busy-Wait for bit to be cleared*/


#endif /* BIT_MANIPULATION_H_ */