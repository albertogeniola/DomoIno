// 
// 
// 

#include "AuthTable.h"

AuthTable::AuthTable() {
	// As first, we need to initialize the auth table.
	// We randomly feed the expected nonce values from other nodes.
	// We also initialize all the nonce values to be sent to other devices to 0.
	for (uint8_t i = 0; i < MAX_NODES; i++) {
		uint32_t rnd = 0;

		do { rnd = RANDOM_REG32; } while (rnd == 0);

		this->rows[i].nonceExpectedFrom = RANDOM_REG32;
		this->rows[i].nonceExpectedFor = 0;
	}

};

uint16_t AuthTable::getExpectedTokenFrom(uint8_t remoteNode) {
	if (remoteNode > MAX_NODES)
		return 0;
	return this->rows[remoteNode].nonceExpectedFrom;
};

void AuthTable::setExpectedTokenFor(uint8_t remoteNode, uint16_t nonce) {
	if (remoteNode > MAX_NODES)
		return;

	this->rows[remoteNode].nonceExpectedFor = nonce;
};

uint16_t AuthTable::getExpectedTokenFor(uint8_t remoteNode) {
	if (remoteNode > MAX_NODES)
		return 0;
	return this->rows[remoteNode].nonceExpectedFor;
};

uint16_t AuthTable::updateExpectedTokenFrom(uint8_t remoteNode) {
	if (remoteNode > MAX_NODES)
		return 0;

	uint32_t rnd = 0;

	do { rnd = RANDOM_REG32; } while (rnd == 0);

	this->rows[remoteNode].nonceExpectedFrom = rnd;
	return rnd;
};