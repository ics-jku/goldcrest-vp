(defconst info-regex "^! \\([-_[:alnum:]]+\\)$")
(defconst label-regex "\\([-_[:alpha:]]+\\):")
(defconst comment-regex "^#\\(.*\\)$")
(defconst instr-header-regex "^@\\([[:alnum:]]+\\)")
(defconst instr-3-regex "^[[:blank:]]*\\([[:alnum:]]+\\)[[:blank:]]+\\([[:alnum:]]+\\)[[:blank:]]+\\([-_[:alnum:]]+\\)")
(defconst instr-2-regex "^[[:blank:]]*\\([[:alnum:]]+\\)[[:blank:]]+\\([[:alnum:]]+\\)")
(defconst instr-1-regex "^[[:blank:]]*\\([[:alnum:]]+\\)")
(defconst c-with-jump-regex "microcode\\[\\([[:blank:]]*[0-9]+\\)\\] = build([[:alnum:]]+, [[:alnum:]]+, \\([-_[:alpha:]]+\\));")

(defconst header-format "#pragma once\n\n#include \"../inc/core.hpp\"\n\nclass %sCore : GoldcrestCore {\npublic:\n  %sCore(Memory *memory): GoldcrestCore(memory) {\n    init();\n  }\n\n  void init() {\n")
(defconst footer-format "  }\n\n  int32_t func(uint32_t address, int32_t value) {\n    return value;\n  }\n\n};")

(defun subleq-parse ()
  (interactive)
  (goto-char (point-min))
  (let ((instructions '()) (name "") (line "") (counter 0) (labels (make-hash-table :test 'equal)))

    (defun comment (line) (add-to-list 'instructions (list 'Comment line) t))
    (defun label (line) (add-to-list 'instructions (list 'Label counter line) t))
    (defun entry (line) (add-to-list 'instructions (list 'Entry counter line) t))

    (defun subleq (src1 src2 jump)
      (add-to-list 'instructions (list 'Subleq counter src1 src2 jump) t)
      (setq counter (1+ counter)))
    (defun instr (src1 src2) (subleq src1 src2 1))
    (defun clean (src1) (subleq src1 src1 1))

    ;; Parse buffer into list structure
    (while (not (eobp))
      (setq line (replace-regexp-in-string "\n" "" (thing-at-point 'line t)))
      (cond
       ((string-match info-regex line)
	(setq name (match-string 1 line)))
       
       ((string-match comment-regex line)
	(comment line))

       ((string-match label-regex line)
	(puthash (match-string 1 line) counter labels)
	(label (match-string 1 line)))
       
       ((string-match instr-header-regex line)
	(entry (match-string 1 line))
	(comment (concat "Instruction " (match-string 1 line)))
	(label (match-string 1 line)))
       
       ((string-match instr-3-regex line)
	(subleq (match-string 1 line) (match-string 2 line) (match-string 3 line)))
       
       ((string-match instr-2-regex line)
	(instr (match-string 1 line) (match-string 2 line)))
       
       ((string-match instr-1-regex line)
	(clean (match-string 1 line))))
      
      (forward-line 1))

    ;; Resolve jumps
    (defun resolve-jump (cmd)
      (if (and (eq 'Subleq (car cmd))
	       (stringp (car (last cmd))))
	  (if (string= "END" (car (last cmd)))
	      (list 'Subleq (nth 1 cmd) (nth 2 cmd) (nth 3 cmd) 255)
	    (let* ((target (gethash (car (last cmd)) labels))
		 (location (nth 1 cmd))
		 (offset (- target location)))
	    (list 'Subleq (nth 1 cmd) (nth 2 cmd) (nth 3 cmd) offset)))
	cmd))

    (list name (mapcar 'resolve-jump instructions))))

(defun string-to-location (str)
  (cond
   ((string= str "SRC1") 2)
   ((string= str "SRC2") 4)
   ((string= str "IMMI") 8)
   ((string= str "IMM") 8)
   ((string= str "TMP0") 0)
   ((string= str "TMP1") 3)
   ((string= str "TMP2") 5)
   ((string= str "TMP3") 6)
   ((string= str "TMP4") 7)
   ((string= str "TMP5") 9)
   ((string= str "RVPC") 1)
   ((string= str "PC") 1)
   ((string= str "INCR") 12)
   ((string= str "INC") 12)
   ((string= str "WORD") 11)
   ((string= str "ONE") 10)
   ;;((string= str "TWO") 12)
   ((string= str "NEXT") 13)
   ((string= str "FUNC0") 14)
   ((string= str "TMP6") 14)
   ((string= str "FUNC1") 15)
   ((string= str "TMP7") 15)
   ((string= str "END") 255)
   (t 0)))


(defun subleq-write-verilog-memdump ()
  (interactive)
  (let* ((parsed (subleq-parse))
	 (name (car parsed))
	 (cmds (cadr parsed))
	 ;(code-buffer (get-buffer-create "*scratch*"))
	 (code-buffer (get-buffer-create (replace-regexp-in-string "sl$" "hex" (buffer-name))))
	 (entry-buffer (get-buffer-create "*subleq-instructions*")))
    (with-current-buffer entry-buffer (erase-buffer))
    (with-current-buffer code-buffer
      (erase-buffer)
      (dolist (cmd cmds)
	(when (eq 'Entry (car cmd))
	  (with-current-buffer entry-buffer
	    (insert (nth 2 cmd) " " (number-to-string (nth 1 cmd)) "\n")))
	(when (eq 'Subleq (car cmd))
	  (insert (format "%04x" (+  (ash (logand 15 (string-to-location (nth 2 cmd))) 12)
				     (ash (logand 15 (string-to-location (nth 3 cmd))) 8)
				     (logand 255 (nth 4 cmd)))) "\n")))
      (while (< (line-number-at-pos) 513) (insert "000\n"))
      (save-buffer))))

(defun subleq-write-c++ ()
  (interactive)
  (let* ((stored-point (point))
	 (parsed (subleq-parse))
	 (name (car parsed))
	 (cmds (cadr parsed)))
    (with-current-buffer (get-buffer-create (replace-regexp-in-string "sl$" "hpp" (buffer-name)))
      (erase-buffer)
      (insert (format header-format name name))
      (dolist (cmd cmds)
	(cond
	 ((eq 'Subleq (car cmd))
	  (let ((line (nth 1 cmd)) (src1 (nth 2 cmd)) (src2 (nth 3 cmd)) (jump (nth 4 cmd)))
	    (cond
	     ((and (eq src1 src2) (eq jump 1)) (insert (format "    microcode[%4d] = clean(%4s);\n" line src1)))
	     ((and (not (eq src1 src2)) (eq jump 1)) (insert (format "    microcode[%4d] = instr(%4s, %4s);\n" line src1 src2)))
	     (t (insert (format "    microcode[%4d] = build(%4s, %4s, %4d);\n" line src1 src2 jump))))))
	 ((eq 'Entry (car cmd))
	  (insert (format "    mapping[%4s] = %d;\n" (nth 2 cmd) (nth 1 cmd))))
	 ((eq 'Comment (car cmd))
	  (insert "    // " (cadr cmd) "\n"))
	 ((eq 'Label (car cmd))
	  (insert "    // " (nth 2 cmd) ":\n"))

	 ))
      (insert footer-format))))

(defun subleq-write-rosette ()
  (interactive)
  (let* ((parsed (subleq-parse))
	 (name (car parsed))
	 (cmds (cadr parsed))
	 (line 0)
	 (entries (make-hash-table :test 'equal)))
    (with-current-buffer (get-buffer-create (replace-regexp-in-string "sl$" "rkt" (buffer-name)))
      (erase-buffer)
      (insert "#lang rosette\n\n")
      (insert "(require \"model.rkt\")\n\n")
      (insert "(define microcode\n  (list\n")
      (dolist (cmd cmds)
	(cond
	 ((eq 'Entry (car cmd))
	  (puthash (nth 2 cmd) line entries))
	 ((eq 'Subleq (car cmd))
	  (insert (format "    (subleq %4s %4s (bv %4s 8))\n" (nth 2 cmd) (nth 3 cmd) (nth 4 cmd)))
	  (setq line (1+ line)))
	 ((eq 'Comment (car cmd))
	  (insert "    ;; " (cadr cmd) "\n"))
	 ((eq 'Label (car cmd))
	  (insert "    ;; " (nth 2 cmd) ":\n"))))
      (delete-backward-char 1)
      (insert "))\n\n")
      ;; Print mapping
      (maphash (lambda (key value)
		 (insert (format "(define %s-PC (bv %s 9))\n" key value))) entries)
      (insert "\n(provide microcode")
      (maphash (lambda (key value) (insert " " key "-PC")) entries)
      (insert ")\n")
      )))

(defun subleq-do ()
  (interactive)
  (let ((out-buffer-name (replace-regexp-in-string "sl$" "hpp" (buffer-name))))
    (with-current-buffer (get-buffer-create out-buffer-name)
      (erase-buffer))
    (subleq-parse out-buffer-name)))

(defun subleq-tosl ()
  (interactive)
  (let ((replacements '(("[[:blank:]]*microcode\\[\\([[:blank:]]*[0-9]+\\)\\][[:blank:]]+=[[:blank:]]+" "\t")
			("build(" "")
			("instr(" "")
			("clean(" "")
			("," "")
			(");" "")
			("[[:blank:]]1[[:blank:]]" " ")
			("[[:blank:]]1[[:blank:]]*" "")
			("//" "#")
			("\\(\\w+[ \t\n]+\\)\\1\\([[:blank:]]*\\)\\(#.*\\)$" "\\1\\2\\3"))))
    (dolist (pair replacements)
      (let ((regex (car pair))
	    (replace (cadr pair)))
	(goto-char (point-min))
	(while (re-search-forward regex nil t) (replace-match replace))))))





;;------------------------------------------------------------

(defconst subleq-highlights
  `(
    ("#.*\n" . font-lock-comment-face)
    ;; Color instruction definitions
    ("@[[:alnum:]]+" . font-lock-string-face)
    ;; Color instruction definitions
    ("! [[:alnum:]]+" . font-lock-type-face)
    ;; Color labels
    ("[-_[:alnum:]]+:" . font-lock-keyword-face)
    ("\\([[:alnum:]]+\\) \\([[:alnum:]]+\\) \\([-[:alnum:]]+\\)" 3 font-lock-keyword-face)
    ;; Color numbers
    (" -?[[:digit:]]+" . font-lock-constant-face)))

(add-to-list 'auto-mode-alist '("\\.sl\\'" . subleq-mode))

;; Create Major Mode
(define-derived-mode subleq-mode fundamental-mode "SUBLEQ Mode"
  ;;:syntax-table subleq-mode-syntax-table
  (setq font-lock-defaults '(subleq-highlights))
  ;;(font-lock-fontify-buffer)
  (local-set-key (kbd "C-c C-c") 'subleq-write-c++)
  (local-set-key (kbd "C-c C-v") 'subleq-write-verilog-memdump)
  (local-set-key (kbd "C-c C-r") 'subleq-write-rosette))

(provide 'subleq)
