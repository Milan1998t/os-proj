/*
 * iddle.h
 *
 *  Created on: Jul 10, 2020
 *      Author: OS1
 */

#ifndef IDDLE_H_
#define IDDLE_H_

#include <iostream.h>


class Iddle : public Thread {
public:
	Iddle() : Thread(128, 1) {}
	void run() {
		while(1);
	}
};
#endif /* IDDLE_H_ */
