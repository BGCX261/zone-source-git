;��������������ļ���ʹ�������ȡ
;(load "addon.el")
      
;;========================================
;; �������
;;�ù������
(mouse-wheel-mode t)
(global-set-key [(control f1)] (quote setnu-mode))
;;��꿿�����ָ��ʱ�������ָ���Զ��ÿ�����ס����
(mouse-avoidance-mode 'animate)
;;ȥ��������
 (tool-bar-mode nil)
;;ȥ���˵������ҽ�F10��Ϊ��ʾ�˵�������һʲô�������ˣ���Ҫ�˵���������F10����������F10��ȥ���˵�
(menu-bar-mode nil)
;;ȥ��������
(if (fboundp 'scroll-bar-mode) (scroll-bar-mode -1))
;;========================================
;;������������
(setq inhibit-startup-message t)

;;========================================
;; ����
;;========================================
;����û��ѡ��һ�������ʱ��C-w����й�������У����ܹ���λ�������
;����M-w��ʱ����Ḵ�ƹ�����ڵ��У����ܹ���λ�������׻�����β�������м��
;����λ�á�������ѡ�������ʱ��C-w��M-w�Ĺ��ܺ�Emacs�Դ���ûɶ������
(defadvice kill-ring-save (before slickcopy activate compile)
 (interactive
 (if mark-active (list (region-beginning) (region-end))
 (list (line-beginning-position)
 (line-beginning-position 2)))))
 
(defadvice kill-region (before slickcut activate compile)
 (interactive
 (if mark-active (list (region-beginning) (region-end))
 (list (line-beginning-position)
 (line-beginning-position 2)))))


;;����һ������
(defun huangq-save-word-at-point()
  (interactive)
  (save-excursion
    (let ((end (progn (unless (looking-back "\>" 1) (forward-word 1)) (point)))
          (beg (progn (forward-word -1) (point))))
      (copy-region-as-kill beg end)
      (message (substring-no-properties (current-kill 0))))))
(global-set-key (kbd "C-c w") 'huangq-save-word-at-point)

;; C-t ���ñ�� ;; 
;; (global-set-key (kbd "C-t") 'set-mark-command)

;; C-x b => CRM bufer list
(global-set-key "\C-xb" 'electric-buffer-list)

;;---------- redo
;; (global-set-key ( kbd "C-.") 'undo) ;; having being C-/


;; F8���ڼ���ת
(global-set-key [f8] 'other-window)


;ȫ��
(defun my-fullscreen ()
(interactive)
(x-send-client-message
nil 0 nil "_NET_WM_STATE" 32
'(2 "_NET_WM_STATE_FULLSCREEN" 0)))
(global-set-key [f11] 'my-fullscreen);F11 ȫ��


;�����֣�Ĭ�ϵĹ���̫�죬�����Ϊ3��
(defun up-slightly () (interactive) (scroll-up 3))
(defun down-slightly () (interactive) (scroll-down 3))
(global-set-key [mouse-4] 'down-slightly)
(global-set-key [mouse-5] 'up-slightly)

;;========================================
;;�رյ�ǰ������ Alt+4  ;; C-x 0
(global-set-key (kbd "M-4") 'delete-window)
;;�ر����������� Alt+1  ;; C-x 1
(global-set-key (kbd "M-1") 'delete-other-windows)
;;ˮƽ�ָ���� Alt+2  ;; C-x 2
(global-set-key (kbd "M-2") 'split-window-vertically)
;;��ֱ�ָ���� Alt+3  ;; C-x 3
(global-set-key (kbd "M-3") 'split-window-horizontally)
;;�л������������� Alt+0 ;; C-x o 
(global-set-key (kbd "M-0") 'other-window)

;;========================================
;; ������
;;========================================

;;�趨�о�
(setq default-line-spacing 0)

;;ҳ�� 
(setq default-fill-column 90)

;;ȱʡģʽ text-mode
(setq default-major-mode 'text-mode)

;;����ɾ����¼
(setq kill-ring-max 200)

;;�Կ��н���
(setq require-final-newline t) 


;;�﷨����
(global-font-lock-mode t)

;;������ʾ����ѡ��
(transient-mark-mode t)

;;ҳ��ƽ�������� scroll-margin 3 ������Ļ����3��ʱ��ʼ���������ԺܺõĿ��������ġ�
(setq scroll-margin 3
      scroll-conservatively 10000)

;������ʾ�ɶ����ţ��������ص���
(show-paren-mode t)
(setq show-paren-style 'parentheses)

;;ճ���ڹ�괦�����������ָ�봦
(setq mouse-yank-at-point t)

;;M-;��
(defun qiang-comment-dwim-line (&optional arg)
  "Replacement for the comment-dwim command.
If no region is selected and current line is not blank and we are not at the end of the line,
then comment current line.
Replaces default behaviour of comment-dwim, when it inserts comment at the end of the line."
  (interactive "*P")
  (comment-normalize-vars)
  (if (and (not (region-active-p)) (not (looking-at "[ \t]*$")))
      (comment-or-uncomment-region (line-beginning-position) (line-end-position))
    (comment-dwim arg)))
(global-set-key "\M-;" 'qiang-comment-dwim-line)

;;========================================
;; ������
;;========================================

;;��������
(setq visible-bell t)

;;ʹ�� y or n ����
(fset 'yes-or-no-p 'y-or-n-p)

;;�����и�
(setq resize-mini-windows nil)

;;�ݹ� minibuffer
(setq enable-recursive-minibuffers t)

;;========================================
;; ״̬��
;;========================================

;;��ʾʱ��
(display-time)
;;ʱ���ʽ
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(setq display-time-interval 10)

;;��ʾ�к�
(setq column-number-mode t)

;;��������ʾ %f ����������·�� %p ҳ��ٷ��� %l �к�
(setq frame-title-format "%f")


;;========================================
;; �༭���趨
;;========================================

;;��������ʱ�ļ�
(setq-default make-backup-files nil)

;;ֻ��Ⱦ��ǰ��Ļ�﷨�������ӿ���ʾ�ٶ�
(setq font-lock-maximum-decoration t)

;;ʹ��X������
(setq x-select-enable-clipboard t) 
;;;;;;;; ʹ�ÿո����� ;;;;;;;;
;; indent-tabs-mode  t ʹ�� TAB ����ʽ���ַ�  nil ʹ�ÿո�����ʽ���ַ�
(setq indent-tabs-mode nil)
(setq tab-always-indent nil)
(setq tab-width 4)
;;����Ĭ��·��
(setq default-directory "~/")

;;========================================
;; ��ɫ����
;;========================================

;; ָ����ɫ
(set-cursor-color "black")
;; �����ɫ
(set-mouse-color "black")
;; ������������ɫ
(set-foreground-color "gainsboro")
(set-background-color "black")
(set-border-color "black")
;; �﷨������ʾ������ѡ�񣬶���ѡ�� ;;ǰ���ͱ���ɫ
(set-face-foreground 'highlight "white")
(set-face-background 'highlight "blue")
(set-face-foreground 'region "cyan")
(set-face-background 'region "blue")
(set-face-foreground 'secondary-selection "skyblue")
(set-face-background 'secondary-selection "darkblue")

;;��ʾ�к�
;; (global-linum-mode t)


;; Load CEDET
(add-to-list 'load-path "~/emacs-23.3/site-lisp/cedet/common")
(require 'cedet)
(require 'semantic-ia)
 
;; Enable EDE (Project Management) features
(global-ede-mode 1)
 
;; Enable SRecode (Template management) minor-mode.
;(semantic-load-enable-minimum-features)
(semantic-load-enable-code-helpers)
;(semantic-load-enable-guady-code-helpers)
;(semantic-load-enable-excessive-code-helpers)
(semantic-load-enable-semantic-debugging-helpers)
;(semantic-idle-completions-mode)
;(setq semantic-default-submodes '(global-semantic-idle-scheduler-mode
;                                  global-semanticdb-minor-mode
;                                  global-semantic-idle-summary-mode
;                                  global-semantic-mru-bookmark-mode))
;(semantic-mode 1)
(global-srecode-minor-mode 1)
;; (setq semanticdb-project-roots (list (expand-file-name "/")))
;;;; CC-mode����  http://cc-mode.sourceforge.net/
(require 'cc-mode)
(c-set-offset 'inline-open 0)
(c-set-offset 'friend '-)
(c-set-offset 'substatement-open 0)
(setq-default indent-tabs-mode  nil)
(setq tab-width 4
      c-basic-offset 4)

;;���ͷ�ļ�Ŀ¼
(defconst cedet-user-include-dirs
  (list ".." "../include" "../inc" "../common" "../public"
        "../.." "../../include" "../../inc" "../../common" "../../public" "/usr/include/" "/usr/local/include/"))
(defconst cedet-win32-include-dirs
  (list "H:/Keil/ARM/INC/Samsung"
        ))
(require 'semantic-c nil 'noerror)
(let ((include-dirs cedet-user-include-dirs))
  (when (eq system-type 'windows-nt)
    (setq include-dirs (append include-dirs cedet-win32-include-dirs)))
  (mapc (lambda (dir)
          (semantic-add-system-include dir 'c++-mode)
          (semantic-add-system-include dir 'c-mode))
        include-dirs))
;;������ת��ݼ�F12
(global-set-key [f12] 'semantic-ia-fast-jump)
;��shift + F12����������
(global-set-key [S-f12]
                (lambda ()
                  (interactive)
                  (if (ring-empty-p (oref semantic-mru-bookmark-ring ring))
                      (error "Semantic Bookmark ring is currently empty"))
                  (let* ((ring (oref semantic-mru-bookmark-ring ring))
                         (alist (semantic-mrub-ring-to-assoc-list ring))
                         (first (cdr (car alist))))
                    (if (semantic-equivalent-tag-p (oref first tag)
                                                   (semantic-current-tag))
                        (setq first (cdr (car (cdr alist)))))
                    (semantic-mrub-switch-tags first))))
;�ں����������Ͷ��崦����
(define-key c-mode-base-map [M-S-f12] 'semantic-analyze-proto-impl-toggle)
;������ȫ�˵�M-n
(define-key c-mode-base-map (kbd "M-m") 'semantic-ia-complete-symbol-menu)
;�����۵�,�����󶨵���[C-c -]��[C-c =]
(require 'semantic-tag-folding nil 'noerror)
(global-semantic-tag-folding-mode 1)
(define-key semantic-tag-folding-mode-map (kbd "C-c -") 'semantic-tag-folding-fold-block)
(define-key semantic-tag-folding-mode-map (kbd "C-c =") 'semantic-tag-folding-show-block)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;������CEDET������;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;setup ecb
(add-to-list 'load-path "~/emacs-23.3/site-lisp/ecb-2.40/")
;(load-file "/usr/share/emacs/site-lisp/ecb-2.32/ecb.el")
(require 'ecb)



(add-to-list 'load-path "~/emacs-23.3/site-lisp/color-theme-6.6.0") 
(require 'color-theme) 
(color-theme-initialize) 
(color-theme-pok-wog)
;(color-theme-oswald)


;; ;; ��Emacs���Tab����: http://emhacks.sourceforge.net/
;; (load-file "~/.emacs.d/tabbar.el")
;; (require 'tabbar)
;; (tabbar-mode 1)
;; ;; (setq tabbar-buffer-groups-function
;; ;; (lambda (buffer)
;; ;; (list "All buffers")))
;; (global-set-key (kbd "M-[") 'tabbar-backward-group)
;; (global-set-key (kbd "M-]") 'tabbar-forward-group)
;; (global-set-key (kbd "M-p") 'tabbar-backward-tab)
;; (global-set-key (kbd "M-n") 'tabbar-forward-tab)
;; (global-set-key [(control tab)] 'tabbar-forward-tab)

;; Use this mode for editing files in the dot-language (www.graphviz.org and
;; http://www.research.att.com/sw/tools/graphviz/).
;; To use graphviz-dot-mode, add 
(load-file "~/.emacs.d/graphviz-dot-mode.el") 
;; http://emacser.com/emacs_graphviz_ds.htm
;; (load "graphviz-dot-mode.el" nil t t)
;; (add-hook 'find-file-hook (lambda()
;;                             (if (string= "dot" (file-name-extension
;;                                                 buffer-file-name))
;;                                 (progn
;;                                   (message "Enabling Setings for dot-mode")
;;                                   (setq fill-column 1000)
;;                                   (base-auto-pair)
;;                                   (local-set-key (kbd "<C-f6>") 'compile)
;;                                   )
;;                               )))

;; (put 'downcase-region 'disabled nil)


(setq gnus-select-method '(nntp "comp.os.linux.answers"))
 ;; or news.cn99.com; news.yaako.com 
(setq mail-sources
      '((pop :server "pop.126.com"   ;; ���������� pop3 ������
             :user "vcprg"            ;; �û���
             :port "pop3"
             :password "YANG89726"))) ;; ���� 

(setq gnus-secondary-select-methods '((nnfolder ""))) ;;�ʼ���ŵĵط�

;; smtpmail.el ���ʼ�

;; (setq user-full-name "YangJie")

;; (setq user-mail-address "yjie_life@163.com")

;; (setq smtpmail-auth-credentials
;; '(("smtp.163.com"
;; 25
;; "yjie_life"
;; "YANG89726")))
;; (setq smtp-ssl "nil")

;; (setq gnus-select-method '(nntp "forums.sybase.com"))
;; ;; (setq smtpmail-default-smtp-server "smtp.163.com")
;; ;; (setq smtpmail-smtp-server "smtp.163.com")
;; ;; (setq message-send-mail-function 'smtpmail-send-it)

;; (setq user-full-name "YangJie") ;; �ⷢ���ʼ������������
;; (setq user-mail-address "vcprg@126.com") ;;�ⷢ���ʼ����������ַ
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com"  ;; SMTP ������
;;        25
;;        "vcprg"          ;; �û���
;;        "YANG89726")))   ;; ����
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

;; ;; �ʼ���������
;; (set-language-environment 'Chinese-GB)
;; (setq gnus-default-charset 'chinese-iso-8bit
;;    gnus-group-name-charset-group-alist '((".*" . chinese-iso-8bit))
;;    gnus-summary-show-article-charset-alist
;;        '((1 . chinese-iso-8bit)
;;          (2 . gbk)
;;          (3 . big5)
;;          (4 . utf-8))
;;    gnus-newsgroup-ignored-charsets
;;        '(unknown-8bit x-unknown iso-8859-1))

;; ;; ��Щ�� web ��ʽ�������ʼ����� html��������������ã�ֻ�����е�plain text ���֣�
;; (eval-after-load "mm-decode"
;;   '(progn
;;      (add-to-list 'mm-discouraged-alternatives "text/html")
;;      (add-to-list 'mm-discouraged-alternatives "text/richtext")))

;; ʹmew��popȡ��ʱ���ż������ڷ�������
(setq mew-pop-delete nil)
;;����������
;; (setq gnus-select-method '(nntp "news.cLinux.ml.org"))
;; (setq gnus-select-method '(nntp "forums.sybase.com"))
(setq gnus-select-method '(nntp "freenews.netfront.net"))
;;(setq gnus-select-method '(nntp "news.newsfan.net"));;���ڵķ�����
;;(setq gnus-select-method '(nntp "news.newsgroup.com.hk"))
;;����guns�ʼ�������
(setq mail-sources
      '((pop :server "pop.126.com"   ;; ���������� pop3 ������
             :user "vcprg"            ;; �û���
             :port "pop3"
             :password "YANG89726"))) ;; ���� 

(setq gnus-secondary-select-methods '((nnfolder "")));;����ʼ��ĵط�

;; (setq user-full-name "���") ;; �ⷢ���ʼ������������
;; (setq user-mail-address "vcprg@126.com") ;;�ⷢ���ʼ����������ַ
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com";; SMTP ������
;;        25
;;        "vcprg"          ;; �û���
;;        "YANG89726")))   ;; ����
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

(setq user-full-name "���") ;; �ⷢ���ʼ������������
(setq user-mail-address "vcprg@126.com") ;;�ⷢ���ʼ����������ַ
(setq smtpmail-auth-credentials
    '(("smtp.126.com";; SMTP ������
       25
       "vpcrg")))          ;; �û���
       ;; "")))   ;; ����
(setq smtpmail-default-smtp-server "smtp.126.com")
(setq smtpmail-smtp-server "smtp.126.com")
(setq message-send-mail-function 'smtpmail-send-it)

;;����
;; (set-language-environment 'Chinese-GB)
(setq gnus-default-charset 'chinese-iso-8bit
   gnus-group-name-charset-group-alist '((".*" . chinese-iso-8bit))
   gnus-summary-show-article-charset-alist
       '((1 . chinese-iso-8bit)
         (2 . gbk)
         (3 . big5)
         (4 . utf-8))
   gnus-newsgroup-ignored-charsets
       '(unknown-8bit x-unknown iso-8859-1))

;;���⣬��Щ�� web ��ʽ�������ʼ����� html��������������ã�ֻ�����е�plain text ���֣�
(eval-after-load "mm-decode"
  '(progn
     (add-to-list 'mm-discouraged-alternatives "text/html")
     (add-to-list 'mm-discouraged-alternatives "text/richtext")))

(setq gnus-default-subscribed-newsgroups
  '("gnu.emacs.help"     ;; ���ﲻ���ޣ��й� emacs ʹ�õ����ⶼ����������
	"fa.linux.kernel"
	"linux.kernel"    
	"comp.os.linux.advocacy"
	"cn.bbs.comp.linux"
	"cn.comp.os.linux")) ;; ����Ҳ��һ�� emacs С�Ż
                         ;; �п�ȥ�ո����ְɣ����ĵ�Ӵ 

