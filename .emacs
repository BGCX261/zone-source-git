;;========================================
;; 脥芒鹿脹脡猫脰脙
;;脫脙鹿枚脰谩脢贸卤锚
(mouse-wheel-mode t)
(global-set-key [(control f1)] (quote setnu-mode))
;;鹿芒卤锚驴驴陆眉脢贸卤锚脰赂脮毛脢卤拢卢脠脙脢贸卤锚脰赂脮毛脳脭露炉脠脙驴陋拢卢卤冒碌虏脳隆脢脫脧脽
;; (mouse-avoidance-mode 'animate)
;;脠楼碌么鹿枚露炉脤玫
(if (fboundp 'scroll-bar-mode) (scroll-bar-mode -1))
;;========================================
;;陆没脫脙脝么露炉禄颅脙忙
(setq inhibit-startup-message t)

;;========================================
;; 录眉掳贸露篓
;;========================================
;碌卤脛茫脙禄脫脨脩隆脰脨脪禄赂枚脟酶脫貌碌脛脢卤潞貌拢卢C-w禄谩录么脟脨鹿芒卤锚脣霉脭脷脨脨拢卢虏禄鹿脺鹿芒卤锚碌脛脦禄脰脙脭脷脛脛脌茂拢禄
;露酶脫脙M-w碌脛脢卤潞貌拢卢脭貌禄谩赂麓脰脝鹿芒卤锚脣霉脭脷碌脛脨脨拢卢虏禄鹿脺鹿芒卤锚碌脛脦禄脰脙脭脷脨脨脢脳禄鹿脢脟脨脨脦虏禄鹿脢脟脨脨脰脨录盲碌脛
;脠脦脪芒脦禄脰脙隆拢碌卤脛茫脫脨脩隆脰脨脟酶脫貌碌脛脢卤潞貌拢卢C-w潞脥M-w碌脛鹿娄脛脺潞脥Emacs脳脭麓酶碌脛脙禄脡露脕陆脩霉隆拢
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


;;赂麓脰脝脪禄赂枚碌楼麓脢
(defun huangq-save-word-at-point()
  (interactive)
  (save-excursion
    (let ((end (progn (unless (looking-back "\>" 1) (forward-word 1)) (point)))
          (beg (progn (forward-word -1) (point))))
      (copy-region-as-kill beg end)
      (message (substring-no-properties (current-kill 0))))))
(global-set-key (kbd "C-c w") 'huangq-save-word-at-point)

;; C-t 脡猫脰脙卤锚录脟 ;; 
;; (global-set-key (kbd "C-t") 'set-mark-command)

;; C-x b => CRM bufer list
;; (global-set-key "\C-xb" 'electric-buffer-list)

;;---------- redo
;; (global-set-key ( kbd "C-.") 'undo) ;; having being C-/


;; F8麓掳驴脷录盲脤酶脳陋
(global-set-key [f8] 'other-window)
(global-set-key (kbd "C-z")'set-mark-command)
(global-set-key (kbd "C-.")'other-window)
(global-set-key [f5] 'revert-buffer)


;脠芦脝脕
(defun my-fullscreen ()
(interactive)
(x-send-client-message
nil 0 nil "_NET_WM_STATE" 32
'(2 "_NET_WM_STATE_FULLSCREEN" 0)))
(global-set-key [f11] 'my-fullscreen);F11 脠芦脝脕


;脢贸卤锚鹿枚脗脰拢卢脛卢脠脧碌脛鹿枚露炉脤芦驴矛拢卢脮芒脌茂赂脛脦陋3脨脨
(defun up-slightly () (interactive) (scroll-up 5))
(defun down-slightly () (interactive) (scroll-down 5))
(global-set-key [mouse-4] 'down-slightly)
(global-set-key [mouse-5] 'up-slightly)

;;========================================
;;鹿脴卤脮碌卤脟掳禄潞鲁氓脟酶 Alt+4  ;; C-x 0
(global-set-key (kbd "M-4") 'delete-window)
;;鹿脴卤脮脝盲脣眉禄潞鲁氓脟酶 Alt+1  ;; C-x 1
(global-set-key (kbd "M-1") 'delete-other-windows)
;;脣庐脝陆路脰赂卯禄潞鲁氓脟酶 Alt+2  ;; C-x 2
(global-set-key (kbd "M-2") 'split-window-vertically)
;;麓鹿脰卤路脰赂卯禄潞鲁氓脟酶 Alt+3  ;; C-x 3
(global-set-key (kbd "M-3") 'split-window-horizontally)
;;脟脨禄禄碌陆脝盲脣眉禄潞鲁氓脟酶 Alt+0 ;; C-x o 
(global-set-key (kbd "M-0") 'other-window)

;;========================================
;; 禄潞鲁氓脟酶
;;========================================

;;脡猫露篓脨脨戮脿
(setq default-line-spacing 0)

;;脪鲁驴铆 
(setq default-fill-column 90)

;;脠卤脢隆脛拢脢陆 text-mode
(setq default-major-mode 'text-mode)

;;脡猫脰脙脡戮鲁媒录脥脗录
(setq kill-ring-max 200)

;;脪脭驴脮脨脨陆谩脢酶
(setq require-final-newline t) 


;;脫茂路篓录脫脕脕
(global-font-lock-mode t)

;;赂脽脕脕脧脭脢戮脟酶脫貌脩隆脭帽
(transient-mark-mode t)

;;脪鲁脙忙脝陆禄卢鹿枚露炉拢卢 scroll-margin 3 驴驴陆眉脝脕脛禄卤脽脩脴3脨脨脢卤驴陋脢录鹿枚露炉拢卢驴脡脪脭潞脺潞脙碌脛驴麓碌陆脡脧脧脗脦脛隆拢
(setq scroll-margin 3
      scroll-conservatively 10000)

;赂脽脕脕脧脭脢戮鲁脡露脭脌篓潞脜拢卢碌芦虏禄脌麓禄脴碌炉脤酶
(show-paren-mode t)
(setq show-paren-style 'parentheses)

;;脮鲁脤霉脫脷鹿芒卤锚麓娄拢卢露酶虏禄脢脟脢贸卤锚脰赂脮毛麓娄
(setq mouse-yank-at-point t)

;;M-;驴矛
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
;; 禄脴脧脭脟酶
;;========================================

;;脡脕脝脕卤篓戮炉
(setq visible-bell t)

;;脢鹿脫脙 y or n 脤谩脦脢
(fset 'yes-or-no-p 'y-or-n-p)

;;脣酶露篓脨脨赂脽
(setq resize-mini-windows nil)

;;碌脻鹿茅 minibuffer
(setq enable-recursive-minibuffers t)

;;========================================
;; 脳麓脤卢脌赂
;;========================================

;;脧脭脢戮脢卤录盲
(display-time)
;;脢卤录盲赂帽脢陆
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(setq display-time-interval 10)

;;脧脭脢戮脕脨潞脜
(setq column-number-mode t)

;;卤锚脤芒脌赂脧脭脢戮 %f 禄潞鲁氓脟酶脥锚脮没脗路戮露 %p 脪鲁脙忙掳脵路脰脢媒 %l 脨脨潞脜
(setq frame-title-format "%f")


;;========================================
;; 卤脿录颅脝梅脡猫露篓
;;========================================

;;虏禄脡煤鲁脡脕脵脢卤脦脛录镁
(setq-default make-backup-files nil)

;;脰禄盲脰脠戮碌卤脟掳脝脕脛禄脫茂路篓赂脽脕脕拢卢录脫驴矛脧脭脢戮脣脵露脠
(setq font-lock-maximum-decoration t)

;;脢鹿脫脙X录么脤霉掳氓
(setq x-select-enable-clipboard t) 
;;;;;;;; 脢鹿脫脙驴脮赂帽脣玫陆酶 ;;;;;;;;
;; indent-tabs-mode  t 脢鹿脫脙 TAB 脳梅赂帽脢陆禄炉脳脰路没  nil 脢鹿脫脙驴脮赂帽脳梅赂帽脢陆禄炉脳脰路没
(setq indent-tabs-mode nil)
;; (setq tab-always-indent nil)
(setq tab-width 4)
;;脡猫脰脙脛卢脠脧脗路戮露
(setq default-directory "~/")

;;========================================
;; 脩脮脡芦脡猫脰脙
;;========================================

;; 脰赂脮毛脩脮脡芦
(set-cursor-color "black")
;; 脢贸卤锚脩脮脡芦
(set-mouse-color "black")
;; 卤鲁戮掳潞脥脳脰脤氓脩脮脡芦
(set-foreground-color "gainsboro")
(set-background-color "black")
(set-border-color "black")
;; 脫茂路篓赂脽脕脕脧脭脢戮拢卢脟酶脫貌脩隆脭帽拢卢露镁麓脦脩隆脭帽 ;;脟掳戮掳潞脥卤鲁戮掳脡芦
(set-face-foreground 'highlight "white")
(set-face-background 'highlight "blue")
(set-face-foreground 'region "cyan")
(set-face-background 'region "blue")
(set-face-foreground 'secondary-selection "skyblue")
(set-face-background 'secondary-selection "darkblue")

;;脧脭脢戮脨脨潞脙
;; (global-linum-mode t)


;; Load CEDET
;; (add-to-list 'load-path "~/.emacs.d/cedet-1.1/common")
;; (load-file "~/.emacs.d/cedet-1.1/common/cedet.el")
;; (load-file "..../cedet/common/cedet.el")
;; (semantic-load-enable-excessive-code-helpers)
;; (require 'cedet)
;; (require 'semantic-ia)
 
;; Enable EDE (Project Management) features
;; (global-ede-mode 1)
 
;; Enable SRecode (Template management) minor-mode.
;; (semantic-load-enable-minimum-features)
;; (semantic-load-enable-code-helpers)
;; (semantic-load-enable-guady-code-helpers)
;(semantic-load-enable-excessive-code-helpers)
;; (semantic-load-enable-semantic-debugging-helpers)
;; (semantic-idle-completions-mode)

(setq semantic-default-submodes '(global-semantic-idle-scheduler-mode
                                 global-semanticdb-minor-mode
                                 global-semantic-idle-summary-mode
                                 global-semantic-mru-bookmark-mode
                                 global-semantic-stickyfunc-mode))
(semantic-mode 1)
(global-semantic-highlight-edits-mode (if window-system 1 -1))
;; (global-semantic-show-unmatched-syntax-mode 1)
(global-semantic-show-parser-state-mode 1)


;; (global-srecode-minor-mode 1)
;; (setq semanticdb-project-roots (list (expand-file-name "/")))
;;;; CC-mode脜盲脰脙  http://cc-mode.sourceforge.net/
(require 'cc-mode)
(c-set-offset 'inline-open 0)
(c-set-offset 'friend '-)
(c-set-offset 'substatement-open 0)
(setq-default indent-tabs-mode nil)
 (setq tab-width 4
       c-basic-offset 4)

;;脤铆录脫脥路脦脛录镁脛驴脗录
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
;;脡猫脰脙脤酶脳陋驴矛陆脻录眉F12
(global-set-key [f12] 'semantic-ia-fast-jump)
;掳麓shift + F12驴脡脗铆脡脧脤酶禄脴
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
;脭脷潞炉脢媒脡霉脙梅麓娄潞脥露篓脪氓麓娄禄楼脤酶
(define-key c-mode-base-map [M-S-f12] 'semantic-analyze-proto-impl-toggle)
;碌炉鲁枚虏鹿脠芦虏脣碌楼M-n
(define-key c-mode-base-map (kbd "M-m") 'semantic-ia-complete-symbol-menu)
;麓煤脗毛脮脹碌镁,掳麓录眉掳贸露篓碌陆脕脣[C-c -]潞脥[C-c =]
;; (require 'semantic-tag-folding nil 'noerror)
;; (global-semantic-tag-folding-mode 1)
;; (define-key semantic-tag-folding-mode-map (kbd "C-c -") 'semantic-tag-folding-fold-block)
;; (define-key semantic-tag-folding-mode-map (kbd "C-c =") 'semantic-tag-folding-show-block)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;脪脭脡脧脢脟CEDET碌脛脜盲脰脙;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;setup ecb
(require 'semantic/analyze)
(provide 'semantic-analyze)
(provide 'semantic-ctxt)
(provide 'semanticdb)
(provide 'semanticdb-find)
(provide 'semanticdb-mode)
(provide 'semantic-load)
;; (add-to-list 'load-path "~/.emacs.d/ecb-snap/")
;; (load-file "~/.emacs.d/ecb-2.40/ecb.el");閿欒\F5\A2\8B\81鐨\84
;; (require 'ecb)

(add-to-list 'load-path "~/.emacs.d/color-theme-6.6.0") 
(require 'color-theme) 
(color-theme-initialize) 
(color-theme-pok-wog)
;; (color-theme-oswald)


;; ;; 赂酶Emacs脤铆录脫Tab鹿娄脛脺: http://emhacks.sourceforge.net/
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
(load-file "~/.emacs.d/cscope-15.8a/contrib/xcscope/xcscope.el")
(require 'xcscope)


(setq gnus-select-method '(nntp "comp.os.linux.answers"))
 ;; or news.cn99.com; news.yaako.com 
(setq mail-sources
      '((pop :server "pop.126.com"   ;; 脭脷脮芒脌茂脡猫脰脙 pop3 路镁脦帽脝梅
             :user "vcprg"            ;; 脫脙禄搂脙没
             :port "pop3"
             :password "YANG89726"))) ;; 脙脺脗毛 

(setq gnus-secondary-select-methods '((nnfolder ""))) ;;脫脢录镁麓忙路脜碌脛碌脴路陆

;; smtpmail.el 路垄脫脢录镁

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

;; (setq user-full-name "YangJie") ;; 脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚脙没脳脰
;; (setq user-mail-address "vcprg@126.com") ;;脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚碌脴脰路
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com"  ;; SMTP 路镁脦帽脝梅
;;        25
;;        "vcprg"          ;; 脫脙禄搂脙没
;;        "YANG89726")))   ;; 脙脺脗毛
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

;; ;; 脫脢录镁脰脨脦脛脡猫脰脙
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

;; ;; 脫脨脨漏脫脙 web 路陆脢陆路垄鲁枚碌脛脫脢录镁脌茂脫脨 html拢卢录脫脠毛脧脗脙忙碌脛脡猫脰脙拢卢脰禄驴麓脝盲脰脨碌脛plain text 虏驴路脰拢潞
;; (eval-after-load "mm-decode"
;;   '(progn
;;      (add-to-list 'mm-discouraged-alternatives "text/html")
;;      (add-to-list 'mm-discouraged-alternatives "text/richtext")))

;; 脢鹿mew脭脷pop脠隆脨脜脢卤陆芦脨脜录镁卤拢脕么脭脷路镁脦帽脝梅脡脧
(setq mew-pop-delete nil)
;;脡猫脰脙脨脗脦脜脳茅
;; (setq gnus-select-method '(nntp "news.cLinux.ml.org"))
;; (setq gnus-select-method '(nntp "forums.sybase.com"))
(setq gnus-select-method '(nntp "freenews.netfront.net"))
;;(setq gnus-select-method '(nntp "news.newsfan.net"));;鹿煤脛脷碌脛路镁脦帽脝梅
;;(setq gnus-select-method '(nntp "news.newsgroup.com.hk"))
;;脡猫脰脙guns脫脢录镁路镁脦帽脝梅
(setq mail-sources
      '((pop :server "pop.126.com"   ;; 脭脷脮芒脌茂脡猫脰脙 pop3 路镁脦帽脝梅
             :user "vcprg"            ;; 脫脙禄搂脙没
             :port "pop3"
             :password "YANG89726"))) ;; 脙脺脗毛 

(setq gnus-secondary-select-methods '((nnfolder "")));;麓忙路脜脫脢录镁碌脛碌脴路陆

;; (setq user-full-name "脩卯陆脺") ;; 脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚脙没脳脰
;; (setq user-mail-address "vcprg@126.com") ;;脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚碌脴脰路
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com";; SMTP 路镁脦帽脝梅
;;        25
;;        "vcprg"          ;; 脫脙禄搂脙没
;;        "YANG89726")))   ;; 脙脺脗毛
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

(setq user-full-name "yangj") ;; 脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚脙没脳脰
(setq user-mail-address "vcprg@126.com") ;;脥芒路垄碌脛脫脢录镁虏脡脫脙脮芒赂枚碌脴脰路
(setq smtpmail-auth-credentials
    '(("smtp.126.com";; SMTP 路镁脦帽脝梅
       25
       "vpcrg")))          ;; 脫脙禄搂脙没
       ;; "")))   ;; 脙脺脗毛
(setq smtpmail-default-smtp-server "smtp.126.com")
(setq smtpmail-smtp-server "smtp.126.com")
(setq message-send-mail-function 'smtpmail-send-it)

;;卤脿脗毛
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

;;脕铆脥芒拢卢脫脨脨漏脫脙 web 路陆脢陆路垄鲁枚碌脛脫脢录镁脌茂脫脨 html拢卢录脫脠毛脧脗脙忙碌脛脡猫脰脙拢卢脰禄驴麓脝盲脰脨碌脛plain text 虏驴路脰拢潞
(eval-after-load "mm-decode"
  '(progn
     (add-to-list 'mm-discouraged-alternatives "text/html")
     (add-to-list 'mm-discouraged-alternatives "text/richtext")))

(setq gnus-default-subscribed-newsgroups
  '("gnu.emacs.help"     ;; 脮芒脌茂虏禄麓铆脿脼拢卢脫脨鹿脴 emacs 脢鹿脫脙碌脛脦脢脤芒露录脭脷脮芒脌茂脤脰脗脹
	"fa.linux.kernel"
	"linux.kernel"    
	"comp.os.linux.advocacy"
	"cn.bbs.comp.linux"
	"cn.comp.os.linux")) ;; 脮芒脌茂脪虏脫脨脪禄赂枚 emacs 脨隆脥脜禄茂拢卢
                         ;; 脫脨驴脮脠楼麓脮赂枚脠脠脛脰掳脡拢卢脰脨脦脛碌脛脫麓 

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(display-time-mode t)
 '(menu-bar-mode nil)
 '(show-paren-mode t)
 '(tool-bar-mode nil))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:family "DejaVu Sans Mono" :foundry "unknown" :slant normal :weight bold :height 98 :width normal)))))

(setq-default indent-tabs-mode nil)

;; (fset 'node-label
;;       "\341\C-k\C-y[]\C-blabel=\"\"\C-b\C-y\C-e;\C-j\C-x\C-s")
;; (global-set-key (kbd "C-2") 'node-label)

;; (fset 'subgraph-cluster-label
;;       [tab ?s ?u ?b ?g ?r ?a ?p ?h ?  ?c ?l ?u ?s ?t ?e ?r ?_ ?\C-. ?\C-s ?\( ?\C-r ?\C-m ?\C-r ?  ?\C-s ?\C-m ?\M-z ?\( ?\C-/ ?\C-. ?\C-y backspace ?\{ return ?\} ?\C-a ?\C-o ?l ?a ?b ?e ?l ?= tab ?\" ?\" ?\C-b ?\C-. ?\C-a ?\M-z ?\{ ?\C-y ?\C-/ ?\C-/ ?\C-. ?\C-y backspace ?\C-e ?\; ?\C-j ?\C-x ?\C-s])
;; (global-set-key (kbd "C-1") 'subgraph-cluster-label)

;; (fset 'parse-cluster
;;    "\C-r \C-s\C-m\C-k\C-y\C-j\C-y ")
;; (global-set-key (kbd "C-3") 'parse-cluster)

;; (load-file "~/.emacs.d/window-number.el")
;; (window-number-mode t)
;; (window-number-meta-mode t)

;; Toggle window dedication

;; http://stackoverflow.com/questions/43765/pin-emacs-buffers-to-windows-for-cscope
(defun toggle-window-dedicated ()

"Toggle whether the current active window is dedicated or not"

(interactive)

(message 

 (if (let (window (get-buffer-window (current-buffer)))

       (set-window-dedicated-p window 

        (not (window-dedicated-p window))))

    "Window '%s' is dedicated"

    "Window '%s' is normal")

 (current-buffer)))
(global-set-key [pause] 'toggle-window-dedicated)

;; 这个不能分辨大小写.
;; (defun zap-upto-char (arg char)
;;   "Kill up to and including ARG'th occurrence of CHAR.
;; Goes backward if ARG is negative; error if CHAR not found."
;;   (interactive "*p\ncZap upto char: ")
;;   (kill-region (point) (progn
;; 			 (search-backward (char-to-string char) nil nil arg)
;; 			 (goto-char (if (> arg 0) (point) (1+ (point))))
;; 			 (point))))
;; (global-set-key (kbd "M-,") 'zap-upto-char)
;; ;; (global-set-key "M-Z" 'zap-upto-char)

(defun copy-upto-char (arg char)
  "Kill up to and including ARG'th occurrence of CHAR.
Goes backward if ARG is negative; error if CHAR not found."
  (interactive "*p\ncCopy upto char: ")
  (copy-region-as-kill (point) (progn
			 (search-backward (char-to-string char) nil nil arg)
			 (goto-char (if (> arg 0) (point) (1+ (point))))
			 (point))))
(global-set-key (kbd "C-,") 'copy-upto-char)


;;赂麓脰脝脪禄赂枚碌楼麓脢
;; (defun huangq-save-token-at-point(count)
;;   (interactiveze)
;;   (save-excursion
;;     (let ((end (progn (search-backward-regexp "[a-zA-Z0-9_]*" nil nil count) (forward-word 1) (point)))
;;           (beg (progn (search-forward-regexp "*[a-zA-Z0-9_]" nil nil count) (forward-word -1) (point))))
;;       (copy-region-as-kill beg end)
;;       (message (substring-no-properties (current-kill 0))))))
;; (global-set-key (kbd "C-c t") 'huangq-save-token-at-point)

(defun huangq-save-token-at-point()
  (interactive)
  (save-excursion
    (let ((end (progn (unless (search-backward-regexp "[^a-zA-Z0-9_]" nil nil nil) (forward-char 1)) (point)))
          ;; (beg (progn (forward-word -1) (point)))
(beg (progn (unless (search-forward-regexp "[a-zA-Z0-9_]*" nil nil nil)) (forward-char 1)(point)))
          )
      (copy-region-as-kill beg end)
      (message (substring-no-properties (current-kill 0))))))
(global-set-key (kbd "C-c t") 'huangq-save-token-at-point)

;; 这个可以分辨大小写
(defun zap-up-to-char (arg char)
  "Kill up to, but not including ARGth occurrence of CHAR.
Case is ignored if `case-fold-search' is non-nil in the current buffer.
Goes backward if ARG is negative; error if CHAR not found.
Ignores CHAR at point."
  (interactive "p\ncZap up to char: ")
  (let ((direction (if (>= arg 0) 0 -1)))
    (kill-region (point)
                 (progn
                   (forward-char direction)
                   (unwind-protect
                       (search-backward (char-to-string char) nil nil arg)
                     (backward-char direction))
                    (point)))))
(global-set-key (kbd "M-,") 'zap-up-to-char)

(add-hook 'dired-mode-hook (lambda ()
  (interactive)
  (make-local-variable  'dired-sort-map)
  (setq dired-sort-map (make-sparse-keymap))
  (define-key dired-mode-map "s" dired-sort-map)
  (define-key dired-sort-map "s" 
              '(lambda () "sort by Size" 
                (interactive) (dired-sort-other (concat dired-listing-switches "S"))))
  (define-key dired-sort-map "x" 
              '(lambda () "sort by eXtension" 
                 (interactive) (dired-sort-other (concat dired-listing-switches "X"))))
  (define-key dired-sort-map "t" 
              '(lambda () "sort by Time" 
                 (interactive) (dired-sort-other (concat dired-listing-switches "t"))))
  (define-key dired-sort-map "n" 
              '(lambda () "sort by Name" 
                 (interactive) (dired-sort-other (concat dired-listing-switches ""))))))


(global-set-key (kbd "M-2") 'windmove-left)          ; move to left windnow
(global-set-key (kbd "M-3") 'windmove-right)        ; move to right window
(global-set-key (kbd "M-p") 'windmove-up)              ; move to upper window
(global-set-key (kbd "M-n") 'windmove-down)          ; move to downer window
