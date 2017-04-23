all:: subject.pdf

finale.tex: ../doc/finale.rst
	pandoc -o finale.tex ../doc/finale.rst

apidoc.tex useapi.tex: ../prologin2017.yml
	stechec2-generator apidoc ../prologin2017.yml .

subject.pdf:: subject.tex subject_content.tex useapi.tex finale.tex apidoc.tex prologin2017.pdf
	xelatex subject.tex

clean:
	rm -f *.toc
	rm -f *.log
	rm -f *.out
	rm -f *.idx
	rm -f *.aux

distclean: clean
	rm -f apidoc.tex
	rm -f useapi.tex
	rm -f finale.tex
	rm -f subject.pdf

.PHONY: all subject.pdf
