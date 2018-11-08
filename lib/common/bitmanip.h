#ifndef BIT_MANIPULATION_H_
#define BIT_MANIPULATION_H_

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))
#define spin_wait_for_set(reg, bit) while(!test_bit(reg, bit))
#define spin_wait_for_clear(reg, bit) while(test_bit(reg, bit))


#endif /* BIT_MANIPULATION_H_ */