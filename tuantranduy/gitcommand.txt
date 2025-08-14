git clone https://github.com/omni-digital/untitled-1.git
# <!-- clone project from github -->
git add .
# <!-- add mọi sự thay đổi vào local -->
git commit -m "commit message"  
# <!-- commit vào local -->
git push origin main
# <!-- push lên github -->


git branch
# <!-- hiển thị tất cả các branch hiện có -->
git checkout -b branch-name
# <!-- tạo branch mới 




git checkout zone-controller
git pull origin zone-controller
git checkout -b feature/smartconfig-support
# make changes...
git add .
git commit -m "feat(wifi): add SmartConfig support"
git push origin feature/smartconfig-support
# Open a PR to merge into zone-controller