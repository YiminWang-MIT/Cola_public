;;
;;  cola-mode.el
;;
;;  Major mode for editing Cola Histogram definitions (*.col)
;;
;; 
;; To use cola-mode, put this in your .emacs file:
;; (autoload 'cola-mode  "~/emacs_lisp/cola-mode" "cola mode" t nil)
;; (autoload 'rundb-mode "~/emacs_lisp/cola-mode" "rundb mode" t nil)
;; (setq auto-mode-alist 
;;      (append (list (cons "\\.col$" 'cola-mode)
;;                    (cons "\\.db$" 'rundb-mode))
;;              auto-mode-alist)) 
;;

(provide 'cola-mode)
(provide 'hm-mode)
(provide 'rundb-mode)

(defun cola-mode ()
  "Cola++ Histogram definitions"
  (c-set-style "cc-mode")
  (local-set-key "\t" 'c-indent-command)
  (local-set-key "{" 'c-electric-brace)
  (local-set-key "}" 'c-electric-brace)
  (setq mode-name "Cola++")
  (setq major-mode 'cola-mode)
)

(defun hm-mode ()
  "Histogram Manager Macro mode"
  (setq mode-name "HM Macro")
  (setq major-mode 'hm-mode)
)

(defun rundb-mode ()
  "Run Database mode"
  (setq mode-name "Cola++ run.db")
  (setq major-mode 'rundb-mode)
)

( hilit-set-mode-patterns
  'cola-mode
  '((hilit-string-find ?' string)
    ("/\\*" "\\*/" comment)
    ("//.*$" nil comment)
    ("^/.*$" nil comment)
    ("\\(File\\|Histogram\\|Scalar\\)" nil keyword)   
    ("\\(Directory\\|Convert\\|Counter\\)" nil keyword)   
    ("\\(Normalize\\|Cut\\|Fourvector\\|Print\\|WriteEvent\\)" nil keyword)
    ("^#[ \t]*\\(undef\\|define\\).*$" "[^\\]$" define)
    ("^#.*$" nil include)
   )
)

( hilit-set-mode-patterns
  'rundb-mode
  '(("/\\*" "\\*/" comment)
    ("\\[" "\\]" keyword)
    ("//.*$" nil comment)
    ("^/.*$" nil comment)
    (hilit-string-find ?' string)
    ("^#[ \t]*\\(undef\\|define\\).*$" "[^\\]$" define)
    ("^#.*$" nil include)
   )
)


