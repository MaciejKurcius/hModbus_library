/**
 * \file		main.c
 * \brief		Main file.
 * \detail		Contain main() function
 * \author		Maciej Kurcius
 * \vesrion		1.0
 * \date		24.11.2021
 */

/* INCUDES */

#include <main.h>

/* VARIABLES */


/* FUNCTION */

int main(void){

	MainLogicInit();
	while (1){
		MainLogicLoop();
		LL_mDelay(MAIN_LOOP_DELAY);
	}
}

/**
  * \brief  This function is executed in case of error occurrence.
  * \param	void
  * \see
  * \note
  * \return None
  */
void Error_Handler(void){
  __disable_irq();
  while (1);
}

#ifdef  USE_FULL_ASSERT
/**
  * \brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * \param  file: pointer to the source file name
  * \param  line: assert_param error line source number
  * \return None
  */
void assert_failed(uint8_t *file, uint32_t line){
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif

/****END OF FILE****/
