lib = loadstring(game:HttpGet"https://pastebin.com/raw/9Cau6Xvm")()
local win = lib:CreateWindow("amh#0001", Vector2.new(492, 598), Enum.KeyCode.RightShift)

local tab1 = win:CreateTab("main")

local sector1 = tab1:CreateSector("main","left")

         

local button = sector1:AddButton("enable amh", function()


    local configs = { -- main aimlock settings
    Main = {
        Enabled = true,
        Aimlockkey = "c",
        Prediction = 0.13093,
        Aimpart = 'HumanoidRootPart', -- Head, UpperTorso, HumanoidRootPart, LowerTorso
        Notifications = true,
        AirshotFunc = true

    },
    Tracer = {
        TracerThickness = 3.5,
        TracerTransparency = 1,
        TracerColor = Color3.fromRGB(99,95,98)
    }

}

local RunService = game:GetService("RunService")
local CurrentCamera = game:GetService "Workspace".CurrentCamera
local Mouse = game.Players.LocalPlayer:GetMouse()
local inset = game:GetService("GuiService"):GetGuiInset().Y

local Line = Drawing.new("Line")
local Plr

Mouse.KeyDown:Connect(function(KeyPressed)
    if KeyPressed == (configs.Main.Aimlockkey) then
        if configs.Main.Enabled == true then
            configs.Main.Enabled = false
            if configs.Main.Notifications == true then
                Plr = FindClosestPlayer()
                game.StarterGui:SetCore("SendNotification", {
                    Title = "amh#0001 | <3",
                    Text = "unlocked",
                    Icon = "http://www.roblox.com/asset/?id=6917008796"
                })
            end
        else
            Plr = FindClosestPlayer()
            configs.Main.Enabled = true
            if configs.Main.Notifications == true then
                game.StarterGui:SetCore("SendNotification", {
                    Title = "amh#0001 | <3",
                    Text = "locked: " .. tostring(LocalPlayer.Character.Humanoid.DisplayName),
                    Icon = "http://www.roblox.com/asset/?id=6917008796"
                })
            end
        end
    end
end)

function FindClosestPlayer()
    local closestPlayer
    local shortestDistance = math.huge

    for i, v in pairs(game.Players:GetPlayers()) do
        if v ~= game.Players.LocalPlayer and v.Character and v.Character:FindFirstChild("Humanoid") and
            v.Character.Humanoid.Health ~= 0 and v.Character:FindFirstChild("HumanoidRootPart") then
            local pos = CurrentCamera:WorldToViewportPoint(v.Character.PrimaryPart.Position)
            local magnitude = (Vector2.new(pos.X, pos.Y) - Vector2.new(Mouse.X, Mouse.Y)).magnitude
            if magnitude < shortestDistance then
                closestPlayer = v
                shortestDistance = magnitude
            end
        end
    end
    return closestPlayer
end

RunService.Stepped:connect(function()
    if configs.Main.Enabled == true then
        local Vector = CurrentCamera:worldToViewportPoint(Plr.Character[configs.Main.Aimpart].Position +
                                                              (Plr.Character.HumanoidRootPart.Velocity *
                                                                  configs.Main.Prediction))
        Line.Color = configs.Tracer.TracerColor
        Line.Thickness = configs.Tracer.TracerThickness
        Line.Transparency = configs.Tracer.TracerTransparency

        Line.From = Vector2.new(Mouse.X, Mouse.Y + inset)
        Line.To = Vector2.new(Vector.X, Vector.Y)
        Line.Visible = true
    elseif configs.Main.Enabled == false then
        Line.Visible = false
    end
end)

local mt = getrawmetatable(game)
local old = mt.__namecall
setreadonly(mt, false)
mt.__namecall = newcclosure(function(...)
    local args = {...}
    if configs.Main.Enabled and getnamecallmethod() == "FireServer" and args[2] == "UpdateMousePos" then
        args[3] = Plr.Character[configs.Main.Aimpart].Position +
                      (Plr.Character[configs.Main.Aimpart].Velocity * configs.Main.Prediction)
        return old(unpack(args))
    end
    return old(...)
end)

if configs.Main.AirshotFunc == true then
    if Plr.Character.Humanoid.Jump == true and Plr.Character.Humanoid.FloorMaterial == Enum.Material.Air then
        settings.config.Part = "RightFoot"
    else
        Plr.Character:WaitForChild("Humanoid").StateChanged:Connect(function(old,new)
            if new == Enum.HumanoidStateType.Freefall then
                settings.config.Part = "RightFoot"
            else
                settings.config.Part = "LowerTorso"
            end
        end)
    end
end



end)

sector1:AddTextbox("enter new pred here",false,function(bool)
    getgenv().Prediction = bool
end)
