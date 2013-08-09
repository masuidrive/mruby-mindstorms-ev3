class EV3
  class Keypad
    def initialize(ev3)
      @ev3 = ev3
    end

    def up?
      ev3.keypad?(0)
    end

    def enter?
      ev3.keypad?(1)
    end

    def down?
      ev3.keypad?(2)
    end

    def right?
      ev3.keypad?(3)
    end

    def left?
      ev3.keypad?(4)
    end

    def back?
      ev3.keypad?(5)
    end
  end

  def keypad
    @keypad ||= Keypad.new(self)
  end
end
