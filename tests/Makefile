all:
	mkdir -p ~/.config
	mkdir -p ~/.config/mpm
	cp -v ../lib/libmpm/tests/conf/base.conf ~/.config/mpm/mpm.conf
	./test.py


add-test:
	cd .. && zsh -c "eval $(CMD) |& cat - > tests/$(DIR)/$(NAME).result"
	echo $(CMD) > $(DIR)/$(NAME).cmd
	echo $(NAME) >> $(DIR)/order

