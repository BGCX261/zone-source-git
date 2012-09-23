;如果有其它配置文件，使此命令读取
;(load "addon.el")
      
;;========================================
;; 外观设置
;;用滚轴鼠标
(mouse-wheel-mode t)
(global-set-key [(control f1)] (quote setnu-mode))
;;光标靠近鼠标指针时，让鼠标指针自动让开，别挡住视线
(mouse-avoidance-mode 'animate)
;;去掉工具栏
 (tool-bar-mode nil)
;;去掉菜单栏，我将F10绑定为显示菜单栏，万一什么东西忘了，需要菜单栏了能摁F10调出，再摁F10就去掉菜单
(menu-bar-mode nil)
;;去掉滚动条
(if (fboundp 'scroll-bar-mode) (scroll-bar-mode -1))
;;========================================
;;禁用启动画面
(setq inhibit-startup-message t)

;;========================================
;; 键绑定
;;========================================
;当你没有选中一个区域的时候，C-w会剪切光标所在行，不管光标的位置在哪里；
;而用M-w的时候，则会复制光标所在的行，不管光标的位置在行首还是行尾还是行中间的
;任意位置。当你有选中区域的时候，C-w和M-w的功能和Emacs自带的没啥两样。
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


;;复制一个单词
(defun huangq-save-word-at-point()
  (interactive)
  (save-excursion
    (let ((end (progn (unless (looking-back "\>" 1) (forward-word 1)) (point)))
          (beg (progn (forward-word -1) (point))))
      (copy-region-as-kill beg end)
      (message (substring-no-properties (current-kill 0))))))
(global-set-key (kbd "C-c w") 'huangq-save-word-at-point)

;; C-t 设置标记 ;; 
;; (global-set-key (kbd "C-t") 'set-mark-command)

;; C-x b => CRM bufer list
(global-set-key "\C-xb" 'electric-buffer-list)

;;---------- redo
;; (global-set-key ( kbd "C-.") 'undo) ;; having being C-/


;; F8窗口间跳转
(global-set-key [f8] 'other-window)


;全屏
(defun my-fullscreen ()
(interactive)
(x-send-client-message
nil 0 nil "_NET_WM_STATE" 32
'(2 "_NET_WM_STATE_FULLSCREEN" 0)))
(global-set-key [f11] 'my-fullscreen);F11 全屏


;鼠标滚轮，默认的滚动太快，这里改为3行
(defun up-slightly () (interactive) (scroll-up 3))
(defun down-slightly () (interactive) (scroll-down 3))
(global-set-key [mouse-4] 'down-slightly)
(global-set-key [mouse-5] 'up-slightly)

;;========================================
;;关闭当前缓冲区 Alt+4  ;; C-x 0
(global-set-key (kbd "M-4") 'delete-window)
;;关闭其它缓冲区 Alt+1  ;; C-x 1
(global-set-key (kbd "M-1") 'delete-other-windows)
;;水平分割缓冲区 Alt+2  ;; C-x 2
(global-set-key (kbd "M-2") 'split-window-vertically)
;;垂直分割缓冲区 Alt+3  ;; C-x 3
(global-set-key (kbd "M-3") 'split-window-horizontally)
;;切换到其它缓冲区 Alt+0 ;; C-x o 
(global-set-key (kbd "M-0") 'other-window)

;;========================================
;; 缓冲区
;;========================================

;;设定行距
(setq default-line-spacing 0)

;;页宽 
(setq default-fill-column 90)

;;缺省模式 text-mode
(setq default-major-mode 'text-mode)

;;设置删除纪录
(setq kill-ring-max 200)

;;以空行结束
(setq require-final-newline t) 


;;语法加亮
(global-font-lock-mode t)

;;高亮显示区域选择
(transient-mark-mode t)

;;页面平滑滚动， scroll-margin 3 靠近屏幕边沿3行时开始滚动，可以很好的看到上下文。
(setq scroll-margin 3
      scroll-conservatively 10000)

;高亮显示成对括号，但不来回弹跳
(show-paren-mode t)
(setq show-paren-style 'parentheses)

;;粘贴于光标处，而不是鼠标指针处
(setq mouse-yank-at-point t)

;;M-;快
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
;; 回显区
;;========================================

;;闪屏报警
(setq visible-bell t)

;;使用 y or n 提问
(fset 'yes-or-no-p 'y-or-n-p)

;;锁定行高
(setq resize-mini-windows nil)

;;递归 minibuffer
(setq enable-recursive-minibuffers t)

;;========================================
;; 状态栏
;;========================================

;;显示时间
(display-time)
;;时间格式
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(setq display-time-interval 10)

;;显示列号
(setq column-number-mode t)

;;标题栏显示 %f 缓冲区完整路径 %p 页面百分数 %l 行号
(setq frame-title-format "%f")


;;========================================
;; 编辑器设定
;;========================================

;;不生成临时文件
(setq-default make-backup-files nil)

;;只渲染当前屏幕语法高亮，加快显示速度
(setq font-lock-maximum-decoration t)

;;使用X剪贴板
(setq x-select-enable-clipboard t) 
;;;;;;;; 使用空格缩进 ;;;;;;;;
;; indent-tabs-mode  t 使用 TAB 作格式化字符  nil 使用空格作格式化字符
(setq indent-tabs-mode nil)
(setq tab-always-indent nil)
(setq tab-width 4)
;;设置默认路径
(setq default-directory "~/")

;;========================================
;; 颜色设置
;;========================================

;; 指针颜色
(set-cursor-color "black")
;; 鼠标颜色
(set-mouse-color "black")
;; 背景和字体颜色
(set-foreground-color "gainsboro")
(set-background-color "black")
(set-border-color "black")
;; 语法高亮显示，区域选择，二次选择 ;;前景和背景色
(set-face-foreground 'highlight "white")
(set-face-background 'highlight "blue")
(set-face-foreground 'region "cyan")
(set-face-background 'region "blue")
(set-face-foreground 'secondary-selection "skyblue")
(set-face-background 'secondary-selection "darkblue")

;;显示行好
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
;;;; CC-mode配置  http://cc-mode.sourceforge.net/
(require 'cc-mode)
(c-set-offset 'inline-open 0)
(c-set-offset 'friend '-)
(c-set-offset 'substatement-open 0)
(setq-default indent-tabs-mode  nil)
(setq tab-width 4
      c-basic-offset 4)

;;添加头文件目录
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
;;设置跳转快捷键F12
(global-set-key [f12] 'semantic-ia-fast-jump)
;按shift + F12可马上跳回
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
;在函数声明处和定义处互跳
(define-key c-mode-base-map [M-S-f12] 'semantic-analyze-proto-impl-toggle)
;弹出补全菜单M-n
(define-key c-mode-base-map (kbd "M-m") 'semantic-ia-complete-symbol-menu)
;代码折叠,按键绑定到了[C-c -]和[C-c =]
(require 'semantic-tag-folding nil 'noerror)
(global-semantic-tag-folding-mode 1)
(define-key semantic-tag-folding-mode-map (kbd "C-c -") 'semantic-tag-folding-fold-block)
(define-key semantic-tag-folding-mode-map (kbd "C-c =") 'semantic-tag-folding-show-block)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;以上是CEDET的配置;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;setup ecb
(add-to-list 'load-path "~/emacs-23.3/site-lisp/ecb-2.40/")
;(load-file "/usr/share/emacs/site-lisp/ecb-2.32/ecb.el")
(require 'ecb)



(add-to-list 'load-path "~/emacs-23.3/site-lisp/color-theme-6.6.0") 
(require 'color-theme) 
(color-theme-initialize) 
(color-theme-pok-wog)
;(color-theme-oswald)


;; ;; 给Emacs添加Tab功能: http://emhacks.sourceforge.net/
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
      '((pop :server "pop.126.com"   ;; 在这里设置 pop3 服务器
             :user "vcprg"            ;; 用户名
             :port "pop3"
             :password "YANG89726"))) ;; 密码 

(setq gnus-secondary-select-methods '((nnfolder ""))) ;;邮件存放的地方

;; smtpmail.el 发邮件

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

;; (setq user-full-name "YangJie") ;; 外发的邮件采用这个名字
;; (setq user-mail-address "vcprg@126.com") ;;外发的邮件采用这个地址
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com"  ;; SMTP 服务器
;;        25
;;        "vcprg"          ;; 用户名
;;        "YANG89726")))   ;; 密码
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

;; ;; 邮件中文设置
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

;; ;; 有些用 web 方式发出的邮件里有 html，加入下面的设置，只看其中的plain text 部分：
;; (eval-after-load "mm-decode"
;;   '(progn
;;      (add-to-list 'mm-discouraged-alternatives "text/html")
;;      (add-to-list 'mm-discouraged-alternatives "text/richtext")))

;; 使mew在pop取信时将信件保留在服务器上
(setq mew-pop-delete nil)
;;设置新闻组
;; (setq gnus-select-method '(nntp "news.cLinux.ml.org"))
;; (setq gnus-select-method '(nntp "forums.sybase.com"))
(setq gnus-select-method '(nntp "freenews.netfront.net"))
;;(setq gnus-select-method '(nntp "news.newsfan.net"));;国内的服务器
;;(setq gnus-select-method '(nntp "news.newsgroup.com.hk"))
;;设置guns邮件服务器
(setq mail-sources
      '((pop :server "pop.126.com"   ;; 在这里设置 pop3 服务器
             :user "vcprg"            ;; 用户名
             :port "pop3"
             :password "YANG89726"))) ;; 密码 

(setq gnus-secondary-select-methods '((nnfolder "")));;存放邮件的地方

;; (setq user-full-name "杨杰") ;; 外发的邮件采用这个名字
;; (setq user-mail-address "vcprg@126.com") ;;外发的邮件采用这个地址
;; (setq smtpmail-auth-credentials
;;     '(("smtp.126.com";; SMTP 服务器
;;        25
;;        "vcprg"          ;; 用户名
;;        "YANG89726")))   ;; 密码
;; (setq smtpmail-default-smtp-server "smtp.126.com")
;; (setq smtpmail-smtp-server "smtp.126.com")
;; (setq message-send-mail-function 'smtpmail-send-it)

(setq user-full-name "杨杰") ;; 外发的邮件采用这个名字
(setq user-mail-address "vcprg@126.com") ;;外发的邮件采用这个地址
(setq smtpmail-auth-credentials
    '(("smtp.126.com";; SMTP 服务器
       25
       "vpcrg")))          ;; 用户名
       ;; "")))   ;; 密码
(setq smtpmail-default-smtp-server "smtp.126.com")
(setq smtpmail-smtp-server "smtp.126.com")
(setq message-send-mail-function 'smtpmail-send-it)

;;编码
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

;;另外，有些用 web 方式发出的邮件里有 html，加入下面的设置，只看其中的plain text 部分：
(eval-after-load "mm-decode"
  '(progn
     (add-to-list 'mm-discouraged-alternatives "text/html")
     (add-to-list 'mm-discouraged-alternatives "text/richtext")))

(setq gnus-default-subscribed-newsgroups
  '("gnu.emacs.help"     ;; 这里不错噢，有关 emacs 使用的问题都在这里讨论
	"fa.linux.kernel"
	"linux.kernel"    
	"comp.os.linux.advocacy"
	"cn.bbs.comp.linux"
	"cn.comp.os.linux")) ;; 这里也有一个 emacs 小团伙，
                         ;; 有空去凑个热闹吧，中文的哟 

