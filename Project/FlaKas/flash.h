#ifndef FLASH_H
#define FLASH_H

#include "module.h"

/**
 * @brief Flashmodul
 *
 * Ein spezifisches Bauteil. Es handelt sich um einen Flash, einen
 * einstufigen Entspannungsverdampfer.
 *
 */

//TODO die GLS vom Module genauer machen, visuelles Zeug

class Flash : public Module
{
private:
    int id;
public:
    Flash();
    int getId();
};

#endif // FLASH_H
