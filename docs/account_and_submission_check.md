# 账号与提交检查记录

## 正确信息

| 项目 | 内容 |
| --- | --- |
| 正确 GitHub 仓库 | `open-vela/contest2026_039_weilaxinghe` |
| 正确开发分支 | `dev-ai-contest-2026` |
| 比赛 GitHub 账号 | `chenzeyuan233` |
| 比赛邮箱 | `13045875890@163.com` |
| 队伍名称 | 维拉星核 |
| 项目名称 | VelaBridge |
| 比赛编号 | 039 |
| 开发板 | 黄山派 SF32LB52 |
| 项目方向 | 基于 openvela 的多模态无障碍 AI 沟通与伴行终端 |

## 检查结果

- 当前仓库远端为 `https://github.com/open-vela/contest2026_039_weilaxinghe.git`，与正确 GitHub 仓库一致。
- 当前工作分支基于 `origin/dev-ai-contest-2026`，共同祖先为官方初始提交。
- 之前 Codex 生成的 bring-up 审查分支已推送到正确仓库的 `feature/review-bringup-records` 分支。
- 通过 GitHub API 检查，当前未发现 `feature/review-bringup-records` 到 `dev-ai-contest-2026` 的 PR，仍需人工在 GitHub 页面创建或确认。
- 文档检索未发现指定的非比赛邮箱或其他非比赛邮箱引用。
- 本次不重写历史 commit author，只统一后续文档、提交说明和本地 git config 建议。
- Codex 连接账号需要人工在 ChatGPT / Codex 设置中确认，仓库文档只能记录正确账号信息，不能代替平台账号切换。

## 本地 Git 配置建议

建议在本地 WSL 或主要开发环境执行：

```bash
git config --global user.name "chenzeyuan233"
git config --global user.email "13045875890@163.com"
```

如果只想先配置当前仓库，可在仓库根目录执行：

```bash
git config user.name "chenzeyuan233"
git config user.email "13045875890@163.com"
```

本次检查已将当前仓库的本地 git config 设置为比赛账号；全局配置和 Codex 平台连接账号仍需人工确认。

## 后续提交要求

- 所有 PR、Issue、README、AI Coding 日志统一使用“维拉星核 / VelaBridge / chenzeyuan233”。
- 提交说明和 PR 描述必须写清测试结果、硬件人工验证项和未完成事项。
- 不重写历史 commit author；如需说明历史提交身份差异，只在后续日志中说明账号统一策略。
- 不修改 `openvela.xml`，不删除官方 `app/hello_app`、`quickapp/hello_quickapp`、`board/contest_board` 模板。
- 不编造喇叭、麦克风、震动马达或 VelaBridge 固件烧录结果。
