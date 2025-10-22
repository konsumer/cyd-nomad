// this will periodically check for incoming messages and save them to SD

void service_lora(void *parameter) {
	while(true) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}