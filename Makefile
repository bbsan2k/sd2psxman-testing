all::
	$(MAKE) -C sd2psxman
	$(MAKE) -C test_app

clean::
	$(MAKE) -C sd2psxman clean
	$(MAKE) -C test_app clean