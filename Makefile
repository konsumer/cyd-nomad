.PHONY: help setup monitor cyd_test upload_cyd_test

# arduino-cli board list
ARDUINO_DEVICE := /dev/ttyUSB0

# arduino-cli board listall esp32:esp32
ARDUINO_BOARD := esp32:esp32:esp32

ARDUINO_CLI_LOCATION := ~/.local/bin
ARDUINO_CLI := ${ARDUINO_CLI_LOCATION}/arduino-cli

help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

setup: ## Install any tools/libs you will need - only run this once
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=${ARDUINO_CLI_LOCATION} sh
	${ARDUINO_CLI} config init
	${ARDUINO_CLI} core update-index
	${ARDUINO_CLI} config set board_manager.additional_urls "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json"


monitor: ## Monitor serial-port
	${ARDUINO_CLI} monitor -p ${ARDUINO_DEVICE} --config 115200


cyd_test: ## Compile cyd_test
	${ARDUINO_CLI} compile --fqbn "${ARDUINO_BOARD}" cyd_test

cyd_test_upload: cyd_test  ## Compile & upload cyd_test
	${ARDUINO_CLI} upload -p "${ARDUINO_DEVICE}" --fqbn "${ARDUINO_BOARD}" cyd_test

test: cyd_test_upload monitor ## quick test that builds, uploads and monitors serial port

nomad: ## Compile cyd_nomad
	${ARDUINO_CLI} compile --fqbn "${ARDUINO_BOARD}" cyd_nomad

nomad_upload: nomad ## Compile & upload cyd_nomad
	${ARDUINO_CLI} upload -p "${ARDUINO_DEVICE}" --fqbn "${ARDUINO_BOARD}" cyd_nomad
