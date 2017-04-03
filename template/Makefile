python = c-micro/server/venv/bin/python

.PHONY: build runserver run clean

.SILENT:
build:
	$(python) c-micro/setup.py install >>c-micro/log.txt
runserver:
	$(python) c-micro/server/run.py
install:
	bash c-micro/server/install.sh
run:
	$(python) c-micro/setup.py install >>c-micro/log.txt && \
	$(python) c-micro/server/run.py
clean:
	rm -rf *~ build/
