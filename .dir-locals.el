((nil . ((eval . (let ((root-path (file-name-directory
				   (let ((d (dir-locals-find-file ".")))
				     (if (stringp d) d (car d))))))
		   (set (make-local-variable 'compile-command)
			;; cmake -H. -Bbuild && make -C build/ >/dev/null && ./run_tests
			(concat "cmake -H" root-path
				" -B" root-path "build"
				" && make -C " root-path "build/ >/dev/null"
                                " && cd " root-path "build/"
                                " && ctest -V"
                                )))))))
